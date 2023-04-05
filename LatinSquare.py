from satispy import Variable, Cnf
from satispy.solver import Minisat

def get_unique_number(r, c, v, n):
    return ((r-1)*n + c - 1)*n + v - 1

def phi_1(n):
    phi_1 = Cnf()
    for v in range(1, n + 1):
        for r in range(1, n + 1):
            clause = Cnf()
            for c in range(1, n + 1):
                clause |= Variable(str(get_unique_number(r, c, v, n)))
            phi_1 &= clause
    return phi_1

def phi_2(n):
    phi_2 = Cnf()
    for v in range(1, n + 1):
        for c in range(1, n + 1):
            clause = Cnf()
            for r in range(1, n + 1):
                clause |= Variable(str(get_unique_number(r, c, v, n)))
            phi_2 &= clause
    return phi_2

def phi_3(n):
    phi_3_1 = Cnf()
    for r in range(1, n + 1):
        for c in range(1, n + 1):
            for v in range(1, n + 1):
                for p in range(v+1, n+1):
                    phi_3_1 &= -Variable(str(get_unique_number(r, c, v, n))) | -Variable(str(get_unique_number(r, c, p, n)))
    phi_3_2 = Cnf()
    for r in range(1, n + 1):
        for c in range(1, n + 1):
            clause = Cnf()
            for v in range(1, n + 1):
                clause |= Variable(str(get_unique_number(r, c, v, n)))
            phi_3_2 &= clause
    return phi_3_1 & phi_3_2

def read_input():
    n = int(input())
    g = [list(map(lambda x : x, input().split())) for r in range(n)]
    return n, g

def hint(n, g):
    hint = Cnf()
    for r in range(n):
        for c in range(n):
            if g[r][c].isnumeric():
                hint &= Variable(str(get_unique_number(r+1, c+1, int(g[r][c]), n)))
    return hint

def ban_solution(sol, n):
    clause = Cnf()
    for r in range(1, n+1):
        for c in range(1, n+1):
            for v in range(1, n+1):
                lit = Variable(str(get_unique_number(r, c, v, n)))
                clause |= -lit if sol[lit] else lit
    return clause

def all_solution():
    n, g = read_input()
    solver = Minisat()
    exp = phi_1(n) & phi_2(n) & phi_3(n) & hint(n, g)

    solution = solver.solve(exp)

    cnt = 0
    while solution.success:
        cnt += 1
        clause = Cnf()
        # for r in range(1, n+1):
        #     for c in range(1, n+1):
        #         for v in range(1, n+1):
        #             print(f'{v} ' if solution[Variable(str(get_unique_number(r, c, v, n)))] else "", end= "")
        #     print()
        exp &= ban_solution(solution, n)
        solution = solver.solve(exp)
    print(f'There are {cnt} solutions')

def one_solution():
    n, g = read_input()
    solution = Minisat().solve(phi_1(n) & phi_2(n) & phi_3(n) & hint(n, g))
    if solution.success:
        print("Found a solution:")
        for r in range(1, n+1):
            for c in range(1, n+1):
                for v in range(1, n+1):
                    print (f'{v} ' if solution[Variable(str(get_unique_number(r, c, v, n)))] else "", end= "")
            print()
    else:
        print("This instance has no solution...")

# Comment/Uncomment one of the following function-calling line of code
# one_solution()
all_solution()