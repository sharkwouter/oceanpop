#include "Board.hpp"

Board::Board(int width, int height) {
   // Create the shells vector
   this->shells.reserve(width);
    for (int x = 0; x < width; x++) {
        this->shells.push_back(std::move(std::vector<Shell>()));
        this->shells[x].reserve(height);
        for (int y = 0; y < height; y++) {
            this->shells[x].push_back(Shell::NONE);
        }
    }

    // Fill the array and remove matches
    while(hasEmpty()) {
        fillEmpty();
        match();
    }
}

std::vector<Shell> Board::match() {
    std::vector<SDL_Point> matchedGems;
    std::vector<Shell> matchesFound = getMatches(this->shells, &matchedGems);

    for (SDL_Point p : matchedGems) {
        this->shells[p.x][p.y] = Shell::NONE;
    }

    return matchesFound;
}

std::vector<Shell> Board::getMatches(std::vector<std::vector<Shell>> shells, std::vector<SDL_Point> * matchedGems=nullptr){
    std::vector<Shell> matchesFound;

    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight() - 2; y++) {
            if (shellsMatch(shells, {x,y},{x,y+1},{x,y+2})) {
                if (matchedGems != nullptr) {
                    matchedGems->push_back({x,y});
                    matchedGems->push_back({x,y+1});
                    matchedGems->push_back({x,y+2});
                }
                matchesFound.push_back(shells[x][y]);
            }
        }
    }
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth() - 2; x++) {
            if (shellsMatch(shells, {x,y},{x+1,y},{x+2,y})) {
                if (matchedGems != nullptr) {
                    matchedGems->push_back({x,y});
                    matchedGems->push_back({x+1,y});
                    matchedGems->push_back({x+2,y});
                }
                matchesFound.push_back(shells[x][y]);
            }
        }
    }

    return matchesFound;
}

void Board::fillEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = (getHeight() - 1); y >= 0; y--) {
            if (this->shells[x][y] != Shell::NONE)
                continue;
            
            // Make the gem above us fall
            if (y > 0) {
                this->shells[x][y] = this->shells[x][y-1];
                this->shells[x][y-1] = Shell::NONE;
            } else {
                dropNewShell(x);
            }
        }
    }
}

void Board::dropNewShell(int x) {
    if (getCount(Shell::BUBBLE) < 6 && (rand() % 20) == 1) {
        this->shells[x][0] = Shell::BUBBLE;
    } else if (getCount(Shell::URCHIN) < 8 && (rand() % 20) == 1) {
        this->shells[x][0] = Shell::URCHIN;
    } else {
        this->shells[x][0] = (Shell) (rand() % ((int) Shell::NUMBER_OF_COLORS - 2));
    }

    // Make sure we're not causing a match directly
    if (!isFalling(x)) {
        if (shellsMatch(this->shells, {x,0},{x,1},{x,2})) {
            dropNewShell(x);
            return;
        }
        if (shellsMatch(this->shells, {x,0},{x+1,0},{x+2,0}) && !isFalling(x+1) && !isFalling(x+2)) {
            dropNewShell(x);
            return;
        }
        if (shellsMatch(this->shells, {x-1,0},{x,0},{x+1,0}) && !isFalling(x-1) && !isFalling(x+1)) {
            dropNewShell(x);
            return;
        }
        if (shellsMatch(this->shells, {x-2,0},{x-1,0},{x,0}) && !isFalling(x-1) && !isFalling(x-2)) {
            dropNewShell(x);
            return;
        }
    }
}

bool Board::isFalling(int x) {
    // Return true for lines which do not exist
    if (!isWithinBounds({x,0})) {
        return true;
    }

    for (int y = 0; y < getHeight(); y++) {
        if (this->shells[x][y] == Shell::NONE) {
            return true;
        }
    }
    return false;
}

bool Board::hasEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            if (this->shells[x][y] == Shell::NONE) {
                return true;
            }
        }
    }
    return false;
}

bool Board::swap(SDL_Point p1, SDL_Point p2) {
    // Make sure the swapped shells are on the same line
    if (p1.x != p2.x && p1.y != p2.y) {
        return false;
    }

    // Make sure the positions are on the board
    if (!isWithinBounds(p1) || !isWithinBounds(p2)) {
        return false;
    }
    
    std::vector<std::vector<Shell>> swappedGems = getShellsAfterSwap(this->shells, p1, p2);
    if (getMatches(swappedGems).size() > 0) {
        this->shells = swappedGems;
        return true;
    } else {
        return false;
    }
}

std::vector<std::vector<Shell>> Board::getShellsAfterSwap(std::vector<std::vector<Shell>> shells, SDL_Point p1, SDL_Point p2) {
    // Return if nothing changed
    if (p1.x == p2.x && p1.y == p2.y) {
        return shells;
    }

    Shell type1 = shells[p1.x][p1.y];
    if (p1.y == p2.y) {
        int direction = ((p2.x - p1.x) > 0) ? 1 : -1;
        for (int i = p1.x + direction; i != p2.x + direction; i+=direction) {
            shells[i-direction][p1.y] = shells[i][p1.y];
            shells[i][p1.y] = type1;
        }
    } else if (p1.x == p2.x) {
        int direction = ((p2.y - p1.y) > 0) ? 1 : -1;
        for (int i = p1.y + direction; i != p2.y + direction; i+=direction) {
            shells[p1.x][i-direction] = shells[p1.x][i];
            shells[p2.x][i] = type1;
        }
    }

    return shells;
}

bool Board::isWithinBounds(SDL_Point point) {
    bool result = true;
    if (point.x < 0 || point.x >= getWidth()) {
        result = false;
    }
    if (point.y < 0 || point.y >= getHeight()) {
        result = false;
    }
    return result;
}

int Board::getCount(Shell shell) {
    int count = 0;
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            if (this->shells[x][y] == shell) {
                count++;
            }
        }
    }
    return count;
}

bool Board::shellsMatch(std::vector<std::vector<Shell>> shells, SDL_Point p1, SDL_Point p2, SDL_Point p3) {
    // Return false if any of the points are not in bounds
    if (!isWithinBounds(p1) || !isWithinBounds(p2) || !isWithinBounds(p3)) {
        return false;
    }

    return (
        shells[p1.x][p1.y] == shells[p2.x][p2.y] &&
        shells[p1.x][p1.y] == shells[p3.x][p3.y]
    );
}

std::vector<std::vector<Shell>> Board::getShellsCopy() {
    std::vector<std::vector<Shell>> copy;
    copy.reserve(getWidth());
    for (int x = 0; x < getWidth(); x++) {
        copy.push_back(std::move(std::vector<Shell>()));
        copy[x].reserve(getHeight());
        for (int y = 0; y < getHeight(); y++) {
            copy[x].push_back(this->shells[x][y]);
        }
    }
    return copy;
}

int Board::getHeight() {
    return shells[0].size();
}

int Board::getWidth() {
    return shells.size();
}
