from persistence import *


def printRow(row):
    mList = list(row)
    for i, parameter in enumerate(mList):
        if isinstance(mList[i], bytes):
            mList[i] = mList[i].decode()
    print(tuple(mList))


def printActivities(employees_id_to_total_income_map):
    print("Activities")
    acts = repo.execute_command(
        """SELECT * FROM activities ORDER BY date ASC""")
    for row in acts:
        if (row[1] < 0):
            product_id = row[0]
            quantity = row[1]
            employee_id = row[2]
            acts = repo.execute_command(
                f"SELECT price FROM products WHERE id = {product_id}")
            product_price = acts[0][0] * -1
            earnings = quantity * product_price
            if employee_id in employees_id_to_total_income_map:
                employees_id_to_total_income_map[employee_id] += earnings
            else:
                employees_id_to_total_income_map[employee_id] = earnings
        printRow(row)


def printBranches():
    print("Branches")
    branches = repo.execute_command(
        """SELECT * FROM branches ORDER BY id ASC""")
    for row in branches:
        printRow(row)


def printEmployees():
    print("Employees")
    emps = repo.execute_command("""SELECT * FROM employees ORDER BY id ASC""")
    for row in emps:
        printRow(row)


def printProducts():
    print("Products")
    prods = repo.execute_command("""SELECT * FROM products ORDER BY id ASC""")
    for row in prods:
        printRow(row)


def printSuppliers():
    print("Suppliers")
    supps = repo.execute_command("""SELECT * FROM suppliers ORDER BY id ASC""")
    for row in supps:
        printRow(row)


def printEmployeesReport(employees_id_to_total_income_map):
    print("\nEmployees report")
    empsByName = repo.execute_command(
        """SELECT e.name,e.salary,b.location FROM employees AS e JOIN branches AS b ON e.branche=b.id  ORDER BY e.name ASC""")

    for row in empsByName:
        for x in row:
            if isinstance(x, bytes):
                x = x.decode('utf_8', 'strict')
            print(x, end=" ")
        name = row[0]
        if isinstance(name, bytes):
            name = name.decode('utf_8', 'strict')
        employee_id = repo.execute_command(
            f"SELECT id FROM employees Where name = '{name}'")[0][0]
        if employee_id in employees_id_to_total_income_map:
            print(employees_id_to_total_income_map[employee_id])
        else:
            print(0)


def printActivityReport():
    print("\nActivities report")
    acts = repo.execute_command(
        """SELECT a.date AS date, p.description, a.quantity, a.activator_id FROM activities AS a JOIN products AS p ON a.product_id=p.id ORDER BY date ASC""")
    for row in acts:
        l = list(row)
        encoded_l = []
        for x in l:
            if isinstance(x, bytes):
                x = x.decode('utf_8', 'strict')
            encoded_l.append(x)
        l = encoded_l
        if (l[2] < 0):
            # sale
            e_id = l[3]
            e_name = repo.execute_command(
                f"SELECT name FROM employees WHERE id = {e_id}")
            name = e_name[0]
            l[3] = name[0].decode('utf_8', 'strict')
            l.append(None)
        else:
            # supply
            s_id = l[3]
            s_name = repo.execute_command(
                f"SELECT name FROM suppliers WHERE id = {s_id}")
            l[3] = None
            l.append(s_name[0][0].decode('utf_8', 'strict'))

        print(tuple(l))


def main():
    employees_id_to_total_income_map = {}

    printActivities(employees_id_to_total_income_map)

    printBranches()

    printEmployees()

    printProducts()

    printSuppliers()

    printEmployeesReport(employees_id_to_total_income_map)

    printActivityReport()


if __name__ == '__main__':
    main()
