#Nagendra Upadhyay. nupadhy3. PA 2 Part 4. 03.21.23
#!/usr/bin/env bash
# This awk command extracts the sequence position and data from each sample file,
# combines them into a single line, sorts the lines by position, and removes the
# position data so that only the sequence data remains. The output is saved to
# sequence.txt.
awk '/^sequence position/ {pos=$3} /^sequence data:/ {seq=pos" "$3; print seq}' *.sample | sort -n | cut -d ' ' -f 2- | tr -d '\n' > sequence.txt

# This line adds a newline character to the end of sequence.txt, if it doesn't already
# have one. This is useful to ensure that the next command (which counts codons)
# works correctly.
echo >> sequence.txt

# This command counts the total number of codons in sequence.txt by dividing the file
# size by 3 (since each codon is 3 characters long).
total_codons=$(( $(wc -c < sequence.txt) / 3 ))

# This command uses grep, sort, and uniq to count the frequency of each unique
# codon in sequence.txt. The output is saved to codons.txt. The awk command formats
# the output as a percentage of the total number of codons in the file.
grep -o ... sequence.txt | sort | uniq -c | awk -v total_codons="$total_codons" '{printf "%s %.6f%%\n", $2, ($1/total_codons)*100}' > codons.txt

# This command uses grep and awk to count the frequency of each nucleotide pair (AT,
# CG, etc.) in sequence.txt. The output is saved to pairs.txt.
grep -o . sequence.txt | awk '
BEGIN {a=0; t=0; c=0; g=0}
{
       if($1 == "A") a++;
       else if($1 == "T") t++;
       else if($1 == "C") c++;
       else if($1 == "G") g++;
}
END {print "A " a"\nT " t"\nC " c"\nG " g}' > pairs.txt
# The awk command initializes four variables (a, t, c, and g) to 0, and then loops
# through each character in the input (which is piped from grep). For each character,
# it increments the corresponding variable.
