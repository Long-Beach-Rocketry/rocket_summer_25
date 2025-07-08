# Script to split raw compacted subscale log data to proper CSV format.

import sys

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Please provide a file name in the first command line argument.")
    else:

        fname = sys.argv[1]

        with open(fname, "r") as r:
            data = [line for group in r.readlines() for line in group.split("|") if line.strip()]

        for line in data:
            print(line)
