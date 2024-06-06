import pandas as pd
import matplotlib.pyplot as plt
import os


def graphs(x, y, title, xlabel, ylabel, filename):
    """
       Функция для построения и сохранения графика.

       Args:
           x (pandas.Series): Данные для оси X.
           y (pandas.Series): Данные для оси Y.
           title (str): Заголовок графика.
           xlabel (str): Подпись оси X.
           ylabel (str): Подпись оси Y.
           filename (str): Название файла, в который будет сохранен график.

       Returns:
           None
       """
    plt.figure(figsize=(10, 6))
    plt.plot(x, y, color='purple', linestyle='-.', linewidth=2, marker='o', markersize=5)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid(True)

    # Получаем путь к текущему каталогу
    current_dir = os.path.dirname(os.path.abspath(__file__))

    # Сохраняем график в текущий каталог
    plt.savefig(os.path.join(current_dir, filename))
    plt.close()


def graf_1():
    """
     Построение первого графика, отображающего зависимость чувствительности приёмника от скорости.

     Returns:
         None
     """
    df1 = pd.read_csv('outputRp.txt', sep=' ', header=None)
    df2 = pd.read_csv('outputrxp.txt', sep=' ', header=None)
    x = df1[0]
    y = df2[0]
    graphs(x, y, 'Зависимость чувствительности приёмника от скорости', 'Значение скорости',
           'Значение чувствительность приёмника', 'graf1.png')


def graf_2():
    """
       Построение второго графика, отображающего зависимость чувствительности приёмника от значения модуляции.

       Returns:
           None
       """
    df1 = pd.read_csv('outputQmp.txt', sep=' ', header=None)
    df2 = pd.read_csv('outputrxp.txt', sep=' ', header=None)
    x = df1[0]
    y = df2[0]
    graphs(x, y, 'Зависимость чувствительности приёмника от значения модуляции', 'Значение модуляции',
           'Значение чувствительности приёмника', 'graf2.png')


print("Графики, которые программа может вывести:")
print("1. Зависимость чувствительности приёмника от скорости")
print("2. Зависимость чувствительности приёмника от значения модуляции")
choice = int(input("Выберите номер графика, который вы хотите увидеть: "))
if choice == 1:
    graf_1()
elif choice == 2:
    graf_2()
else:
    print("Неверный ввод.")
