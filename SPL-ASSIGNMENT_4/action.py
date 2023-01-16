from persistence import *
from typing import List

import sys


def main(args: List[str]):
    inputfilename: str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline: list[str] = line.strip().split(", ")
            # TODO: apply the action (and insert to the table) if possible


if __name__ == '__main__':
    main(sys.argv)
