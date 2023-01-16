from persistence import *
from typing import List

import sys

def add_activity(splittedline: List[str]):
    product_id, quantity, activator_id, date = splittedline
    repo.execute_command(
        f"INSERT INTO activities values ({product_id}, {quantity}, {activator_id}, {date})")
    
    pass
### still not completed
def main(args: List[str]):
    inputfilename: str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline: list[str] = line.strip().split(", ")
            add_activity(splittedline)
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
                isInsertionLegal = currentQuantity + quantity >= 0
            else:
                isInsertionLegal = True
            if (isInsertionLegal):
                add_activity(splittedline)
                print("action insertion has been done successfully")
            else:
                print("insertion is illegal")
                continue

            # TODO: apply the action (and insert to the table) if possible
    

    
    
    




if __name__ == '__main__':
    main(sys.argv)
