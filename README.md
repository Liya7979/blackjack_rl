# Using Reinforcemenet Learning for Blackjack game

## Getting started

Please install the ```requirements.txt``` file if you do not have Numpy, Matplotlib or SciPy packages using the following command:
```
pip install -r requirements.txt
```

## Running the program

To run the program, use the bash script ```get_results.sh``` in your terminal (you may need to install UNIX shell if you 
are using [Windows](https://itsfoss.com/install-bash-on-windows/):
```
bash get_results.sh
```
or
```
chmod +x get_results.sh 
./get_results.sh
```

The program performs 10 runs of each RL algorithm automatically. While running, the program will output to stdout the number
of iterations passed in 100k. After the C++ code is done running, the Python script for plotting the data will be executed
automatically. You can see the plots in the ```blackjack/plots``` folder. It make take some time to obtain the results (usually around 10-15 minutes)
The plots already use the averaged results 
over 10 runs. For the optimal policy plots, the name of the file represents the name of the algorithm used.

## Authors
* Liya Yeshmagambetova
* Michaela Mrazkova
