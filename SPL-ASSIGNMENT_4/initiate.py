from persistence import *
from typing import List

import sys
import os


def add_branche(splittedline: List[str]):
    id, location, number_of_employees = splittedline
    repo.execute_command(
        f"INSERT INTO branches values ({id}, '{location}', {number_of_employees})")
    pass


def add_supplier(splittedline: List[str]):
    id, name, contact_information = splittedline
    repo.execute_command(
        f"INSERT INTO suppliers values ({id}, '{name}', '{contact_information}')")

    pass


def add_product(splittedline: List[str]):
    id, description, price, quantity = splittedline
    repo.execute_command(
        f"INSERT INTO products values ({id}, '{description}', {price}, {quantity})")

    pass


def add_employee(splittedline: List[str]):
    id, name, salary, branche_id = splittedline
    repo.execute_command(
        f"INSERT INTO employees values ({id}, '{name}', {salary}, {branche_id})")

    pass


adders = {"B": add_branche,
          "S": add_supplier,
          "P": add_product,
          "E": add_employee}


def main(args: List[str]):
    inputfilename = args[1]
    # delete the database file if it exists
    repo._close()
    # uncomment if needed
    if os.path.isfile("bgumart.db"):
        os.remove("bgumart.db")
    repo.__init__()
    repo.create_tables()
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline: List[str] = line.strip().split(",")
            adders.get(splittedline[0])(splittedline[1:])


if __name__ == '__main__':
    main(sys.argv)
