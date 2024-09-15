#Nagendra Upadhyay. nupadhy3. PA 2 Part 3. 03.21.23
#!/usr/bin/env bash

awk -F',' '     # Use awk with comma as field separator
    NR > 1 {    # Skip first line of the input file
        make = $1   # Extract the first column into the variable "make"
        hp = $6     # Extract the sixth column into the variable "hp"
        if (hp > max_hp) {  # If "hp" is greater than "max_hp"
            max_hp = hp     # Set "hp" as the new "max_hp"
            max_make = make     # Update "max_make" to the corresponding make
        }
    }
    END {   # After reading all lines in the input file
        printf("%s %.6f\n", max_make, max_hp) > "answer.txt" 
    }
' cars.csv
