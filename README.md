From original repository:

BigMC (Bigraphical Model Checker) is a model-checker designed to operate on Bigraphical Reactive Systems (BRS). BRS is a formalism developed by Robin Milner and colleagues that emphasises the orthogonal notions of locality and connectivity. Bigraphs have found applications in ubiquitous computing, computational biology, business workflow modelling, and context-aware systems.

By model checking, we mean precisely the act of checking whether some specification is true of a particular bigraphical model. This is achieved through a kind of exhaustive search of all the possible states of the system. For arbitrary models, this kind of checking is computationally intractable, as the state space is simply too huge (and indeed infinite in many cases). The challenge of this kind of task is to limit the kinds of models we check to some tractable subset, and to reduce the number of actual states that we need to check directly in order to provide concrete correctness guarantees.

Thanks to @AlessandroCaste below is a list of dependencies required to build and run this tool on ubuntu-like systems. Tested on LUbuntu 18.10.

### Dependencies

```
sudo apt-get install autoconf
sudo apt-get install libtool
sudo apt-get install google-perftools
sudo apt-get install flex bison
sudo apt-get install libreadline-dev
sudo apt-get install texinfo
sudo apt-get install g++ 
```

### Installation
```
git clone
cd bigmc
./autogen.sh
./configure
make
sudo make install
```

### Export
```
export PATH=/usr/local/bigmc/bin:$PATH
export BIGMC_HOME=/usr/local/bigmc
```

### Running
Type <code>bigmc</code> in terminal to run the tool.

