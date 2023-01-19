from persistence import *
from typing import List

import sys


def handle_sale(splittedline: List[str]):
    product_id, quantity, activator_id, date = splittedline
    repo.execute_command(
        f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")
    act = repo.execute_command(
        f"SELECT quantity FROM products WHERE id = {int(product_id)}")
    currentQuantity = act[0][0]
    repo.execute_command(
        f"UPDATE products SET quantity={currentQuantity - int(quantity)} WHERE id = {int(product_id)}")


def handle_supply(splittedline: List[str]):
    product_id, quantity, activator_id, date = splittedline
    repo.execute_command(
        f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")
    act = repo.execute_command(
        f"SELECT quantity FROM products WHERE id = {int(product_id)}")
    currentQuantity = act[0][0]
    repo.execute_command(
        f"UPDATE products SET quantity={currentQuantity + int(quantity)} WHERE id = {int(product_id)}")


def main(args: List[str]):
    inputfilename: str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline: list[str] = line.strip().split(", ")
            quantity = int(splittedline[1])

            if quantity == 0:
                continue
            isSale = quantity < 0
            productId = int(splittedline[0])

            ids_quantities_table = repo.execute_command(
                f"SELECT id,quantity FROM products WHERE id = {productId}")
            currentQuantity = ids_quantities_table[0][1]

            if (isSale):
                if (currentQuantity + quantity >= 0):
                    handle_sale(splittedline)
                else:
                    print("insertion is illegal")
                    continue
            else:
                handle_supply(splittedline)


if __name__ == '__main__':
    main(sys.argv)
