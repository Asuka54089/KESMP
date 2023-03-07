# k-Best Egalitarian Stable Marriages for Task Assignment (KESMP)

# Introduction

In a two-sided market with each agent ranking individuals on the other side according to their preferences, such as location or incentive. The stable marriage problem calls to find a perfect matching among the two sides such that no pair of agents prefers each other to their assigned matches. Recent studies show that the number of solutions can be large in practice. Yet the classic solution by the Gale-Shapley (GS) algorithm is optimal for agents on the one side and pessimal for those on the other side. Other algorithms find a stable marriage that optimizes a quality measure. However, in many real-world circumstances, a decision-maker needs to examine a set of good solutions and choose among them based on expert knowledge. With such a disposition, one needs to find a set of high quality stable marriages. In this paper, we provide efficient algorithms that find the k-best stable marriages by egalitarian cost, i.e., the cumulative satisfaction of agents. Our exhaustive experimental study using real-world data and realistic preferences demonstrates the efficacy and efficiency of our solution.


# Datasets

- BIKE
- FOOD
- ADM
- HARD

# Methods

### Exact Algorithms:

ENUM

ENUM*

ENUM*-L

ENUM*-R

ENUM*-LR

### Heuristic Algorithms:

ENUM*-ɛ

ENUM*-LR-ɛ
