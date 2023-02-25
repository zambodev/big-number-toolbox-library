import sys
import random


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print(f"Argument missing: {sys.argv[0]} < nbits[] >")
        quit()

    with open("/tmp/bitstr.txt", "w") as f:
        f.write(''.join(random.choice('01') for _ in range(sum(map(int, sys.argv[1::])))))      # It's unreadable but I like it

    print("Done")