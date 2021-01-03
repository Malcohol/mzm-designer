# mzm-designer
A level-design assistant for creating MazezaM levels

## INTRODUCTION

The software is a suite of tools for solving and designing MazezaM puzzles. 
I wrote this around 2010 to help create the extra levels I shipped with
[1k MazezaM](https://github.com/Malcohol/1kMazezaM).

I'm still very proud of the original 10 levels I shipped with [the original MazezaM](https://github.com/Malcohol/ZXMazezaM) back in 2002.
However, creating levels solely by hand is very time-consuming, since potential levels have to be carefully playtested to ensure they don't have easy solutions.
The mzm-designer assistant lets you experiment with a level design, while it solves the level in the background and gives it a rough quality score.

```
┌────────────────────┬────────── MazezaM Designer ─────────────────────────────┐
│       Editor       │                                                         │
│                    │                                                         │
│ q quit             │                                                         │
│ s save             │                                                         │
│ -/+ improver-/+    │                                                         │
│ p play             │                                                         │
│ v view             │                         ########                        │
│ I improve          │                         #  $  $#                        │
│ b block            │                         # $ $$ #                        │
│ g gap              │                         #$ $ $ *                        │
│ s save             │                         +  $$ $#                        │
│ SPACE toggle       │                         ########                        │
│ DEL delete         │                                                         │
│ BS backspace       │                                                         │
│ </> shift l/r      │                                                         │
│ {/} height -/+     │                                                         │
│ [/] width -/+      │                                                         │
│ m(M) (un)select    │                                                         │
│ a(A) (un)selectAll ├─────────────────────────────────────────────────────────┤
│ C clear            │ Level number:       1*      Rating:           2.63      │
│ R randomize        │ Improver radius:     5      # Moves:            46      │
│                    │                             # Pushes:           19      │
└────────────────────┴─────────────────────────────────────────────────────────┘
```

It is possible to use this code to entirely automate the process of creating levels, but I've found that such levels are rarely fun. 
They tend to be too difficult, and are not amenable to high-level reasoning.
(It is likely that a very clever metric or machine learning could overcome this, but that's beyond the scope of this project.)
The last level of the 1k MazezaM level-set (level 30, "The Beast") is an example of a primarily automated level, and I personally find it horribly difficult.

## Important disclaimer

Prior to pushing this to gitHub, I did only minor testing of mzm-designer.
Other than that, I had neither built nor run this program in about 10 years.
Since it was primarily an internal project, there may be bugs and pitfalls I do not remember.

## Included projects

mzmslv - a library which offers some standard search algorithms (e.g.
  depth first, breadth first, A*) and a system for multithreading work
  across multiple threads.

mzm - A library of classes for manipulating, solving and importing
  MazezaM puzzles.

mzmcommon - A library containing some general tools.

mzm-solve - An non-interactive program which will solve MazezaM puzzles.

mzm-designer - An interactive program to help design MazezaM puzzles.

## Dependencies

The UI is written using the ncurses library.
I don't remember the reason :/

## LICENSE

All software is licensed under the GNU General Public License version 3.
