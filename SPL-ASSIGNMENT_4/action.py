from persistence import *
from typing import List

import sys

# def add_activity(splittedline: List[str]):
#     product_id, quantity, activator_id, date = splittedline
#     repo.execute_command(
#         f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")
#     print("insertion has been done successfully")
#     pass

def handle_sale(splittedline: List[str]):
    product_id, quantity, activator_id, date = splittedline
    repo.execute_command(f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")
    getPrice = repo.execute_command(f"SELECT id,price FROM products WHERE id = {productId}")
    price = getPrice[0][1]
    emp_id = int(activator_id)
    quan = int(quantity)
    income = price*quan
    repo.execute_command(f"UPDATE empTotalIncome SET total_income= {income} WHERE employee_id = {emp_id}" )

def handle_buy(splittedline: List[str]):
    product_id, quantity, activator_id, date = splittedline
    repo.execute_command(f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")



    
## still not completed
def main(args: List[str]):
    inputfilename: str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline: list[str] = line.strip().split(", ")
            #add_activity(splittedline)
            quantity = int(splittedline[1])

            #print (quantity)

            if quantity == 0:
                continue
            isSale = quantity < 0
            productId = int(splittedline[0])

            #print(productId)

            ids_quantities_table = repo.execute_command(f"SELECT id,quantity FROM products WHERE id = {productId}")
            currentQuantity = ids_quantities_table[0][1]

            #print(currentQuantity)

            isInsertionLegal = False

            #print(isInsertionLegal)
            
            if (isSale):
                if (currentQuantity + quantity >= 0):
                    handle_sale(splittedline)
                else:
                    print("insertion is illegal")
                    continue
            else:
                handle_buy(splittedline)
           

            # TODO: apply the action (and insert to the table) if possible
    

    
    
    




if __name__ == '__main__':
    main(sys.argv)
