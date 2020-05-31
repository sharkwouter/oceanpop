# Match Theory Design Document

This document contains the basic rules of the game and some of the code design.

## Basic concept

Match Theory is a match 3 game. The player moves gems to make matches of 3 or more on a board to score points.

## Game Loop

This is how the basic game loop will look like approximately:

![](/doc/diagram/gameloop.png)

## Game Rules

The game has the following rules:
- Gems can be moved either horizontall or vertically, not both.
- Gems can only be moved by making matches.
- Gems can be moved as far as the player would like.
