from collections import defaultdict
from os import path

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable


def read_file(filename):
    qtable = defaultdict(float)
    with open(path.dirname(__file__) + '/../' +filename) as file:
        for line in file:
            numbers = line.split(",")
            arr = list(map(int, numbers[:4]))
            (a, b, c, d) = (arr[0], arr[1], arr[2], arr[3])
            qtable[(a, b, c, d)] = float(numbers[4])
    return qtable


def read_data(filename):
    data = []
    with open(path.dirname(__file__) + '/../' +filename) as file:
        for line in file:
            data.append(float(line))
    return data


def plot_rewards(ql, sarsa, td, plot_name="plot.png"):
    algorithms = [ql, sarsa, td]
    x = [i * 1000 for i in range(len(ql))]
    labels = ['Q-learning', 'Sarsa', 'TD(0)']
    for i in range(len(algorithms)):
        plt.plot(x, algorithms[i], label=labels[i])
    plt.ylabel('Average reward')
    plt.xlabel('Episodes')
    plt.legend()
    plt.savefig(path.join(outpath, plot_name))


def plot_policy(policy, plot_filename="plot.png"):
    def get_Z(player, dealer, usable_ace):
        hit_or_stick = lambda hit_q_val, stick_q_val: 1 if hit_q_val >= stick_q_val else 0
        return hit_or_stick(policy[player, dealer, usable_ace, 0],
                            policy[player, dealer, usable_ace, 1])

    def get_figure(usable_ace, ax):
        x_range = np.arange(1, 11)
        y_range = np.arange(11, 22)
        X, Y = np.meshgrid(x_range, y_range)
        Z = np.array([[get_Z(player_hand, dealer_showing, usable_ace) for dealer_showing in x_range] for player_hand in
                      range(21, 10, -1)])
        surf = ax.imshow(Z, cmap=plt.get_cmap('Accent', 2), vmin=0, vmax=1, extent=[0.5, 10.5, 10.5, 21.5])
        plt.xticks(x_range, ('A', '2', '3', '4', '5', '6', '7', '8', '9', '10'))
        plt.yticks(y_range)
        ax.set_xlabel('Dealer Showing')
        ax.set_ylabel('Player Hand')
        ax.grid(color='black', linestyle='-', linewidth=1)
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.1)
        cbar = plt.colorbar(surf, ticks=[0, 1], cax=cax)
        cbar.ax.set_yticklabels(['0 (STICK)', '1 (HIT)'])
        cbar.ax.invert_yaxis()

    fig = plt.figure(figsize=(15, 15))
    ax = fig.add_subplot(121)
    ax.set_title('Usable Ace', fontsize=16)
    get_figure(True, ax)
    ax = fig.add_subplot(122)
    ax.set_title('No Usable Ace', fontsize=16)
    get_figure(False, ax)
    plt.savefig(path.join(outpath, plot_filename))
    plt.show()


outpath = path.dirname(__file__) + '/../plots/'
losses_ql = read_data("Q-learning_rewards.txt")
losses_sarsa = read_data("Sarsa_rewards.txt")
losses_td = read_data("TD_rewards.txt")
plot_rewards(losses_ql, losses_sarsa, losses_td, "learning_curve")
qtable = read_file("ql.txt")
plot_policy(qtable, "qtable_ql")
qtable = read_file("sarsa.txt")
plot_policy(qtable, "qtable_sarsa")
qtable = read_file("td.txt")
plot_policy(qtable, "qtable_td")
# eps = read_data("Sarsa_eps.txt")
# plt.plot(eps)
# plt.show()
