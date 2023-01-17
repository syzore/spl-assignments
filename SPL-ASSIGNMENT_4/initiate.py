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
    repo.execute_command(
        f"INSERT INTO empTotalIncome values ({id}, 0)")

    pass






adders = {"B": add_branche,
          "S": add_supplier,
          "P": add_product,
          "E": add_employee}


