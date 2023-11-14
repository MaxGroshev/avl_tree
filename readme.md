# AVL tree
- This program helps to find ranges of elements during the O(log(n)), where n- number of elements

Required programs:

-  Cmake version  3.21


# Install and run
```
> git clone git@github.com:MaxGroshev/avl_tree.git
> mkdir build
> cd build
```
#### How to run my_avl_tree?

```
> cmake ..
> make
> /avl_tree/avl_tree

```

#### How to run std::set?

```
> cmake ..
> make
> /avl_tree/set

```

# Tests
Required programs:

- Python
- Google tests


### How to run unit tests?
```
> enable testing
> make tests
> ./tests/unit_tests
```
---

### How to run end to end tests?
Run this command from top project directory
```
> python ./tests/end_to_end_tests/e2e_test.py

```

# Test generator
Required programs:

```
> python3 ./tests/end_to_end_tests/test_generator.py --num 40000 -vd -656 49090  -lub 5355  7585 --file ./tests/end_to_end_tests/my_test_dat/19.dat

```
 - -n  [--num] sets num of elements in test
 - -vd sets the range of data for the test
 - lub sets lower and upper bound for searching of range
 - -f [--file] sets name of file for data

#### How to run test generator?
```
> enable testing
> make tests
> ./tests/unit_tests
```
---

# Performance
- insert, search runs during O(log(n))

<p align="center">
  <img width="600" height="520" src="https://github.com/MaxGroshev/avl_tree/blob/master/efficiency_comp/tree_d1.png">
</p>

## Efficiency comparison with std:set

<p align="center">
  <img width="600" height="400" src="https://github.com/MaxGroshev/avl_tree/blob/master/efficiency_comp/start_graph.png">
</p>

<p align="center">
  <img width="600" height="400" src="https://github.com/MaxGroshev/avl_tree/blob/master/efficiency_comp/graph.png">
</p>

---
