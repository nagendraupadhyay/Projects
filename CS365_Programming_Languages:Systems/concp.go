//Nagendra Upadhyay. CS365 pa4.
package main

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"
)

// Progress represents the progress of file copying
type Progress struct {
	sync.Mutex
	progressMap map[string]float64
}

// FileProgress represents the progress of an individual file
type FileProgress struct {
	File    string
	Percent float64
}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Usage: concp <file1> <file2> ... <destination_directory>")
		os.Exit(1)
	}

	destDir := os.Args[len(os.Args)-1]
	filePaths := os.Args[1 : len(os.Args)-1]

	destInfo, err := os.Stat(destDir)
	if err != nil {
		fmt.Println("Error:", err)
		os.Exit(1)
	} else if !destInfo.IsDir() {
		fmt.Println("Error: Destination is not a directory")
		os.Exit(1)
	}

	errCh := make(chan error)
	var wg sync.WaitGroup
	progress := &Progress{
		progressMap: make(map[string]float64),
	}
	fileProgress := make(chan FileProgress)

    // Launch goroutines to copy each file concurrently
	for _, filePath := range filePaths {
		wg.Add(1)

		go func(filePath string) {
			defer wg.Done()
            // Check if source file exists and is not a directory
			fileInfo, err := os.Stat(filePath)
			if err != nil {
				errCh <- fmt.Errorf("Error: %s does not exist", filePath)
				return
			} else if fileInfo.IsDir() {
				errCh <- fmt.Errorf("Error: %s is a directory", filePath)
				return
			}
            // Open source file
			srcFile, err := os.Open(filePath)
			if err != nil {
				errCh <- fmt.Errorf("Error: Failed to open %s", filePath)
				return
				defer srcFile.Close()
			}
            // Create destination file
			destPath := filepath.Join(destDir, filepath.Base(filePath))
			destFile, err := os.Create(destPath)
			if err != nil {
				errCh <- fmt.Errorf("Error: Failed to create destination file %s", destPath)
				return
				defer destFile.Close()
			}

			fileSize := float64(fileInfo.Size())
			buffer := make([]byte, 1024)
			var bytesRead float64

            // Copy data from source file to destination file
			for {
				n, err := srcFile.Read(buffer)
				if err != nil && err != io.EOF {
					errCh <- fmt.Errorf("Error: Failed to read from %s", filePath)
					return
				}

				if n == 0 {
					break
				}
				_, err = destFile.Write(buffer[:n])
				if err != nil {
					errCh <- fmt.Errorf("Error: Failed to write to %s", destPath)
					return
				}

				bytesRead += float64(n)
                // Update the progress for the file
				progress.Lock()
				progress.progressMap[filePath] = (bytesRead / fileSize) * 100
				progress.Unlock()

                // Send the file progress to the channel
				fileProgress <- FileProgress{
					File:    filePath,
					Percent: (bytesRead / fileSize) * 100,
				}
			}

            // Remove the file progress from the map once the file is copied completely
			progress.Lock()
			delete(progress.progressMap, filePath)
			progress.Unlock()
		}(filePath)
	}

    go func() {
        progressMap := make(map[string]float64)
        completedFiles := make(map[string]bool)
        completedCount := 0

        for fileProgress := range fileProgress {
            progressMap[fileProgress.File] = fileProgress.Percent

            // Check if all files have completed
            allFilesComplete := true
            for _, filePath := range filePaths {
                if _, ok := completedFiles[filePath]; !ok {
                    if percent, exists := progressMap[filePath]; exists && percent == 100 {
                        completedFiles[filePath] = true
                        completedCount++
                    } else {
                        allFilesComplete = false
                    }
                }
            }

            // Output progress for each file
            output := ""
            for _, filePath := range filePaths {
                if percent, ok := progressMap[filePath]; ok {
                    output += fmt.Sprintf("%s: %.2f%%\n", filePath, percent)
                }
            }
            fmt.Println(output)

            // Break if all files have completed
            if allFilesComplete && completedCount == len(filePaths) {
                break
            }
        }

        // Close the errCh channel after all files have completed
        for range filePaths {
            errCh <- nil
        }
    }()

    wg.Wait()             // Wait for all goroutines to complete
    close(errCh)          // Close the errCh channel
    close(fileProgress)   // Close the fileProgress channel
    fmt.Println("Copy completed successfully to the directory!")
}
