import matplotlib.pyplot as plt
import os


def parse_filename(name):
    new_string = ""
    for character in name:
        if character.isdigit():
            new_string += character
        elif character == "_" and "0." not in name:
            new_string += "."
    return new_string


def linear_versus_geo(geo_name, linear_name, time_chart=True):
    with open("factor/" + geo_name, "r") as file:
        x = []
        y_geo = []
        lines = file.readlines()
        for line in lines:
            n, time, success_rate = line.split(";")
            if int(n) < 150:
                x.append(int(n))
                if time_chart:
                    y_geo.append(float(time))
                else:
                    y_geo.append(float(success_rate))
    with open("step/" + linear_name, "r") as file:
        y_lin = []
        lines = file.readlines()
        for line in lines:
            n, time, success_rate = line.split(";")
            if int(n) < 150:
                if time_chart:
                    y_lin.append(float(time))
                else:
                    y_lin.append(float(success_rate))
    if time_chart:
        title = "Wpływ schematu chłodzenia na czas wykonywania algorytmu"
        plt.title(title)
    else:
        title = "Wpływ schematu chłodzenia na poprawność znalezionych rozwiązań"
        plt.title(title)
    plt.plot(x, y_geo, "o-", label="geometryczny")
    plt.plot(x, y_lin, "o-", label="liniowy")
    plt.xlabel("Liczba wierzchołków")
    if time_chart:
        plt.ylabel("Czas - s")
    else:
        plt.ylabel("Błąd względny znalezionego rozwiązania - %")
    plt.legend()
    plt.savefig(title.replace(" ", "_"))
    plt.figure()


def comparison():
    # Held Karp
    x_1 = [6, 10, 12, 13, 14, 17, 21, 24, 26]
    y_1 = [0, 0, 0, 0, 0, 0, 2, 20, 90]
    # Brute force
    x_2 = [6, 10, 12, 13, 14]
    y_2 = [0, 0, 20, 80, 1200]
    with open("factor/factor0_99.txt") as file:
        lines = file.readlines()
        x_3 = []
        y_3 = []
        for line in lines:
            n, time, success_rate = line.split(";")
            if int(n) < 50:
                y_3.append(float(time))
                x_3.append(int(n))
    title = "Porównanie algorytmów do rozwiązywania problemu TSP"
    plt.title(title)
    plt.plot(x_1, y_1, "o-", label="Held Karp")
    plt.plot(x_2, y_2, "o-", label="Brute force")
    plt.plot(x_3, y_3, "o-", label="Simulated annealing")
    plt.ylabel("Czas - s")
    plt.xlabel("Liczba wierzchołków")
    plt.legend()
    plt.savefig(title.replace(" ", "_"))
    plt.figure()


def create_chart(dirname, time_chart=True, title=None, maximum_vertices=1000):
    files = os.listdir(dirname)
    for file in files:
        parameter = parse_filename(file)
        with open(os.path.join(dirname, file), "r") as file:
            x = []
            y = []
            lines = file.readlines()
            for line in lines:
                n, time, success_rate = line.split(";")
                if int(n) < maximum_vertices:
                    x.append(int(n))
                    if time_chart:
                        y.append(float(time))
                    else:
                        y.append(float(success_rate))
        plt.plot(x, y, "o-", label=parameter)
    if title:
        plt.title(title)
    plt.xlabel("Liczba wierzchołków")
    if time_chart:
        plt.ylabel("Czas - s")
    else:
        plt.ylabel("Błąd względny znalezionego rozwiązania - %")
    plt.legend()
    plt.savefig(title.replace(" ", "_") + str(maximum_vertices)[0])
    plt.figure()


def starting_temperature(time_chart=True):
    with open("starting_temperature/starting_temperature30.txt", "r") as file:
        x = []
        y_1 = []
        lines = file.readlines()
        for line in lines:
            n, time, success_rate = line.split(";")
            x.append(int(n))
            if time_chart:
                y_1.append(float(time))
            else:
                y_1.append(float(success_rate))
    with open("starting_temperature/starting_temperature900.txt", "r") as file:
        y_2 = []
        lines = file.readlines()
        for line in lines:
            n, time, success_rate = line.split(";")
            if time_chart:
                y_2.append(float(time))
            else:
                y_2.append(float(success_rate))
    if time_chart:
        title = "Wpływ temperatury startowej na czas wykonywania algorytmu"
    else:
        title = "Wpływ temperatury startowej na jakość otrzymywanych rozwiązań"
    plt.title(title)
    plt.plot(x, y_1, "o-", label="30 * liczba_wierzchołków")
    plt.plot(x, y_2, "o-", label="30 * liczba_wierzchołków^2")
    plt.xlabel("Liczba wierzchołków")
    if time_chart:
        plt.ylabel("Czas - s")
    else:
        plt.ylabel("Błąd względny znalezionego rozwiązania - %")
    plt.legend()
    plt.savefig(title.replace(" ", "_"))
    plt.figure()


starting_temperature()
starting_temperature(False)
# comparison()
# linear_versus_geo("factor0_99.txt", "step0_3.txt")
# linear_versus_geo("factor0_99.txt", "step0_3.txt", time_chart=False)
# create_chart(
#     "factor", time_chart=True, title="Wpływ parametru a na czas wykonywania algorytmu"
# )
# create_chart(
#     "factor",
#     time_chart=False,
#     title="Wpływ parametru a na poprawność znalezionych rozwiązań",
# )
# create_chart(
#     "factor",
#     time_chart=True,
#     title="Wpływ parametru a na czas wykonywania algorytmu",
#     maximum_vertices=80,
# )
# create_chart(
#     "factor",
#     time_chart=False,
#     title="Wpływ parametru a na poprawność znalezionych rozwiązań",
#     maximum_vertices=80,
# )
# create_chart(
#     "max_epoch",
#     time_chart=True,
#     title="Wpływ parametru max_epoch na czas wykonywania algorytmu",
# )
# create_chart(
#     "max_epoch",
#     time_chart=False,
#     title="Wpływ parametru max_epoch na poprawność znalezionych rozwiązań",
# )
# create_chart(
#     "max_epoch",
#     time_chart=True,
#     title="Wpływ parametru max_epoch na czas wykonywania algorytmu",
#     maximum_vertices=80,
# )
# create_chart(
#     "max_epoch",
#     time_chart=False,
#     title="Wpływ parametru max_epoch na poprawność znalezionych rozwiązań",
#     maximum_vertices=80,
# )
# create_chart(
#     "step",
#     time_chart=True,
#     title="Wpływ parametru step na czas wykonywania algorytmu",
# )
# create_chart(
#     "step",
#     time_chart=False,
#     title="Wpływ parametru step na poprawność znalezionych rozwiązań",
# )
