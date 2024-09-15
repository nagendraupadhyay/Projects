#Nagendra Upadhyay. nupadhy3. PA 2 Part 2. 03.21.23
#!/usr/bin/env bash

# Initialize variables to store the maximum horsepower and the make of the car with the max horsepower
max_hp=0
max_make=""
  
# Loop through each line in the cars.csv file
while read -r line; do
    # Skip the first line (header)
    if [[ $line == Make* ]]; then
        continue
    fi
  
    # Extract the make (1st column) and horsepower (6th column) from the line using cut
    make=$(echo "$line" | cut -d "," -f 1)
    hp=$(echo "$line" | cut -d "," -f 6)
  
    # Compare the extracted horsepower value with the stored max value
    if [ $(echo "$hp > $max_hp" | bc) -eq 1 ]; then
        # If it's larger, set the new max and make
            max_hp=$hp
            max_make=$make
    fi
done < cars.csv
 
# Output the max make and horsepower to answer.txt in the correct format
echo "$max_make $max_hp" > answer.txt
