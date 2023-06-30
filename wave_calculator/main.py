import math


def sin(x):
    return 2.5 * math.sin(x) + 2.5


def abs_sin(x):
    return sin(x/2)


def square_wave(x):
    while not (0 <= x < 2 * math.pi):
        x -= math.pi
    if math.pi <= x < 2 * math.pi:
        return 5
    else:
        return 0


def triangle_wave(x):
    while not (0 <= x < 2 * math.pi):
        x -= math.pi
    if math.pi <= x < 2 * math.pi:
        return 5 - (5 / math.pi) * (x - math.pi)
    else:
        return (5 * x) / math.pi


def step_wave(x):
    if x == math.pi:
        return 4
    return math.floor(triangle_wave(x))


def tooth_saw_wave(x):
    return (5 * x) / (2 * math.pi)


input_range = [(i * math.pi * 2) / 100 for i in range(100)]
sin_range = [sin(i) for i in input_range]
abs_sin_range = [abs_sin(i) for i in input_range]
square_range = [square_wave(i) for i in input_range]
triangle_range = [triangle_wave(i) for i in input_range]
step_range = [step_wave(i) for i in input_range]
tooth_saw_range = [tooth_saw_wave(i) for i in input_range]

print(f"""input_range = {input_range};
sin_range = {sin_range};
abs_sin_range = {abs_sin_range};
square_range = {square_range};
triangle_range = {triangle_range};
step_range = {step_range};
tooth_saw_range = {tooth_saw_range};""")
