from satispy import Variable, Cnf
from satispy.solver import Minisat

def get_unique_number(r, c, C):
    return (r-1)*C + c - 1

def append_zeroes(s, cnt):
    return '0' * cnt + s

def phi_1(m, n, y):
    phi_1_1 = Cnf()
    for i in range(m):
        for J in range(2**n):
            bin_rep = bin(J)[2:]
            bin_rep = append_zeroes(bin_rep, n - len(bin_rep))
            if bin_rep.count('1') == y[i] + 1:
                clause = Cnf()
                for j, char in enumerate(bin_rep):
                    if char == '1':
                        clause |= -Variable(str(get_unique_number(i+1, j+1, n)))
                phi_1_1 &= clause
    phi_1_2 = Cnf()
    for i in range(m):
        for J in range(2**n):
            bin_rep = bin(J)[2:]
            bin_rep = append_zeroes(bin_rep, n - len(bin_rep))
            if bin_rep.count('1') == n - y[i] + 1:
                clause = Cnf()
                for j, char in enumerate(bin_rep):
                    if char == '1':
                        clause |= Variable(str(get_unique_number(i+1, j+1, n)))
                phi_1_2 &= clause
    return phi_1_1 & phi_1_2

def phi_2(m, n, x):
    phi_2_1 = Cnf()
    for j in range(n):
        for I in range(2**m):
            bin_rep = bin(I)[2:]
            bin_rep = append_zeroes(bin_rep, n - len(bin_rep))
            if bin_rep.count('1') == x[j] + 1:
                clause = Cnf()
                for i, char in enumerate(bin_rep):
                    if char == '1':
                        clause |= -Variable(str(get_unique_number(i+1, j+1, n)))
                phi_2_1 &= clause
    phi_2_2 = Cnf()
    for j in range(n):
        for I in range(2**m):
            bin_rep = bin(I)[2:]
            bin_rep = append_zeroes(bin_rep, n - len(bin_rep))
            if bin_rep.count('1') == m - x[j] + 1:
                clause = Cnf()
                for i, char in enumerate(bin_rep):
                    if char == '1':
                        clause |= Variable(str(get_unique_number(i+1, j+1, n)))
                phi_2_2 &= clause
    return phi_2_1 & phi_2_2

def read_input():
    m, n = map(int, input().split())
    Y = list(map(int, input().split()))
    X = list(map(int, input().split()))
    return m, n, Y, X

def ban_solution(sol, m, n):
    clause = Cnf()
    for r in range(1, m+1):
        for c in range(1, n+1):
            lit = Variable(str(get_unique_number(r, c, n)))
            clause |= -lit if sol[lit] else lit
    return clause

def all_solution():
    m, n, Y, X = read_input()
    solver = Minisat()
    exp = phi_1(m, n, Y) & phi_2(m, n, X)

    solution = solver.solve(exp)

    cnt = 0
    while solution.success:
        cnt += 1
        print(f'[{cnt}]')
        clause = Cnf()
        for i in X:
            print(i, end=" ")
        print()
        print('- ' * n)
        for r in range(1, m+1):
            for c in range(1, n+1):
                print (f'1' if solution[Variable(str(get_unique_number(r, c, n)))] else '0', end= " ")
            print(f'| {Y[r-1]}')
        exp &= ban_solution(solution, m, n)
        solution = solver.solve(exp)
    print(f'There {"is" if cnt <= 1 else "are"} {cnt} solution{"s" if cnt > 1 else ""}')

def one_solution():
    m, n, Y, X = read_input()
    solution = Minisat().solve(phi_1(m, n, Y) & phi_2(m, n, X))
    if solution.success:
        print("Found a solution:")
        for i in X:
            print(i, end=" ")
        print()
        print('- ' * n)
        for r in range(1, m+1):
            for c in range(1, n+1):
                # print (f'1' if solution[Variable(str(get_unique_number(r, c, n)))] else '0', end= " ")
                if solution[Variable(str(get_unique_number(r, c, n)))]:
                    print(1, end=" ")
                else:
                    print(0, end=" ")
            print(f'| {Y[r-1]}')
    else:
        print("This instance has no solution...")

one_solution()
# all_solution()