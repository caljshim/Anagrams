input_file = "words_alpha.txt"
output_file = "words.txt"

with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    for line in infile:
        word = line.strip()
        if 0 < len(word) <= 6:
            outfile.write(word + "\n")