from persistence import *


def printActivities():
    print("Activities")
    acts = repo.execute_command(
        """SELECT * FROM activities ORDER BY date ASC""")
    for row in acts:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printBranches():
    print("Branches")
    branches = repo.execute_command(
        """SELECT * FROM branches ORDER BY id ASC""")
    for row in branches:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printEmployees():
    print("Employees")
    emps = repo.execute_command("""SELECT * FROM employees ORDER BY id ASC""")
    for row in emps:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printProducts():
    print("Products")
    prods = repo.execute_command("""SELECT * FROM products ORDER BY id ASC""")
    for row in prods:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printSuppliers():
    print("Suppliers")
    supps = repo.execute_command("""SELECT * FROM suppliers ORDER BY id ASC""")
    for row in supps:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printEmployeesReport():
    print("Employees report")
    # need to add total sales income
    empsByName = repo.execute_command(
        """SELECT e.name,e.salary,b.location FROM employees AS e JOIN branches AS b ON e.branche=b.id  ORDER BY e.name ASC""")
    for row in empsByName:
        tmp = (str)(row).replace("b'", "")
        tmp = tmp.replace("'", "")
        print(tmp)


def printActivityReport():
    print("Activity report")
    # need to relate to sales/supplier case and update the table
    acts = repo.execute_command(
        """SELECT a.date AS date, p.description, a.quantity, a.activator_id FROM activities AS a JOIN products AS p ON a.product_id=p.id ORDER BY date ASC""")
    for row in acts:
        l = list(row)
        if (l[2] < 0):
            # sale
            e_id = l[3]
            e_name = repo.execute_command(
                f"SELECT name FROM employees WHERE id = {e_id}")
            name = e_name[0]
            l[3] = name[0]
            l.append(None)
        else:
            # supply
            s_id = l[3]
            s_name = repo.execute_command(
                f"SELECT name FROM suppliers WHERE id = {s_id}")
            l[3] = None
            l.append(s_name[0][0])

        # tmp =(str)(row).replace("b'", "")
        # tmp = tmp.replace("'","")
        print(l.__str__())


def main():
    printActivities()

    printBranches()

    printEmployees()

    printProducts()

    printSuppliers()

    printEmployeesReport()

    printActivityReport()

    # TODO: implement
    pass


if __name__ == '__main__':
    main()
