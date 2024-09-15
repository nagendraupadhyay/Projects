#Nagendra Upadhyay. nupadhy3. PA 2 Part 1. 03.21.23
#!/usr/bin/env bash
# Loop over every personal record file in the current directory
for file in *.txt; do
  
    # Extract the name from the file name
    name=$(basename "$file" .txt)
  
    # Create a directory for the name
    mkdir "$name"
  
    # Extract each property from the record and echo it into the appropriate file
    # Extract 'id' field from file and save it to a new file named id.txt in the directory created above
    grep "id" "$file" | cut -d " " -f 2 > "$name/id.txt"
    # Extract 'age' field from file and save it to a new file named age.txt in the directory created above
    grep "age" "$file" | cut -d " " -f 2 > "$name/age.txt"
    # Extract 'email' field from file and save it to a new file named email.txt in the directory created above
    grep "email" "$file" | cut -d " " -f 2 > "$name/email.txt"
  
    # Remove the old personal record file
    rm "$file"
done
