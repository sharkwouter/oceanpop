#include "Board.hpp"

Board::Board(int width, int height, int seed, bool isRelaxedMode) {
    this-> seed = seed;
    this->isRelaxedMode = isRelaxedMode;
    // Create the shells vector
    this->shells.reserve(width);
    for (int x = 0; x < width; x++) {
        this->shells.push_back(std::move(std::vector<ShellType>()));
        this->shells[x].reserve(height);
        for (int y = 0; y < height; y++) {
            this->shells[x].push_back(ShellType::NONE);
        }
    }

    // Fill the array and remove matches
    while(hasEmpty()) {
        dropShells();
        match();
    }
}

Board::Board(std::vector<std::vector<ShellType>> shells, int seed, bool isRelaxedMode) {
    this->seed = seed;
    this->isRelaxedMode = isRelaxedMode;
    this->shells = shells;
}

Board::~Board() {
    for (int x = 0; x < getWidth(); x++) {
        this->shells[x].clear();
    }
    this->shells.clear();
}

std::vector<Match> Board::match() {
    std::vector<Match> matchesFound = getMatches(this->shells);

    for (Match m : matchesFound) {
        if (m.match_type == MatchType::HORIZONTAL) {
            for(int x = 0; x < 3; x++) {
                this->shells[m.x+x][m.y] = ShellType::NONE;
            }
        } else if (m.match_type == MatchType::VERTICAL)  {
            for(int y = 0; y < 3; y++) {
                this->shells[m.x][m.y+y] = ShellType::NONE;
            }
        }
    }

    return matchesFound;
}

std::vector<Match> Board::getMatches(std::vector<std::vector<ShellType>> shells){
    std::vector<Match> matchesFound;
    std::vector<Match> verticalMatches;
    std::vector<Match> horizontalMatches;
    std::vector<Match> bonusMatches;

    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight() - 2; y++) {
            if (shellsMatch(shells, {x,y},{x,y+1},{x,y+2})) {
                verticalMatches.push_back({shells[x][y], x, y, MatchType::VERTICAL});
            }
        }
    }
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth() - 2; x++) {
            if (shellsMatch(shells, {x,y},{x+1,y},{x+2,y})) {
                horizontalMatches.push_back({shells[x][y], x, y, MatchType::HORIZONTAL});
            }
        }
    }

    // Find bonus matches, aka shells that are in both a horizontal and vertical  match
    for (Match horizontal : horizontalMatches) {
        for (Match vertical : verticalMatches) {
            for (int h = 0; h < 3; h++) {
                for (int v = 0; v < 3; v++) {
                    if (horizontal.x + h == vertical.x && horizontal.y == vertical.y + v) {
                        if (horizontal.type == ShellType::BUBBLE) {
                            continue;
                        }
                        if (horizontal.type == ShellType::URCHIN) {
                            continue;
                        }
                        bonusMatches.push_back({horizontal.type, horizontal.x + h, vertical.y + v, MatchType::BONUS});
                    }
                }
            }
        }
    }

    matchesFound.reserve(verticalMatches.size() + horizontalMatches.size() + bonusMatches.size());
    matchesFound.insert(matchesFound.end(), verticalMatches.begin(), verticalMatches.end());
    matchesFound.insert(matchesFound.end(), horizontalMatches.begin(), horizontalMatches.end());
    matchesFound.insert(matchesFound.end(), bonusMatches.begin(), bonusMatches.end());

    return matchesFound;
}

std::vector<Shell> Board::dropShells() {
    std::vector<Shell> droppingShells;
    for (int x = 0; x < getWidth(); x++) {
        for (int y = (getHeight() - 1); y >= 0; y--) {
            if (this->shells[x][y] != ShellType::NONE)
                continue;
            
            // Make the gem above us fall
            if (y > 0) {
                this->shells[x][y] = this->shells[x][y-1];
                this->shells[x][y-1] = ShellType::NONE;
            } else {
                dropNewShell(x);
            }
            droppingShells.push_back({this->shells[x][y], x, y-1});
        }
    }
    return droppingShells;
}

void Board::dropNewShell(int x) {
    int total_shells = getHeight() * getWidth();
    if (getCount(ShellType::BUBBLE) < std::min(6,total_shells/9) && (this->rand() % 20) == 1) {
        this->shells[x][0] = ShellType::BUBBLE;
    } else if (!this->isRelaxedMode && getCount(ShellType::URCHIN) < std::min(8,total_shells/7) && (this->rand() % 20) == 1) {
        this->shells[x][0] = ShellType::URCHIN;
    } else {
        this->shells[x][0] = (ShellType) (this->rand() % ((int) ShellType::NUMBER_OF_COLORS - 2));
    }

    // Make sure we're not causing a match directly
    if (shellsMatch(this->shells, {x,0},{x,1},{x,2})) {
        dropNewShell(x);
        return;
    }

    // Horizontal matches should only be checked at the top
    if (!isFalling(x)) {
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
        if (this->shells[x][y] == ShellType::NONE) {
            return true;
        }
    }
    return false;
}

bool Board::hasEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            if (this->shells[x][y] == ShellType::NONE) {
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
    
    std::vector<std::vector<ShellType>> swappedGems = getShellsAfterSwap(this->shells, p1, p2);
    if (getMatches(swappedGems).size() > 0) {
        this->shells = swappedGems;
        return true;
    } else {
        return false;
    }
}

std::vector<std::vector<ShellType>> Board::getShellsAfterSwap(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2) {
    // Return if nothing changed
    if (p1.x == p2.x && p1.y == p2.y) {
        return shells;
    }

    ShellType type1 = shells[p1.x][p1.y];
    if (p1.y == p2.y) {
        int match_type = ((p2.x - p1.x) > 0) ? 1 : -1;
        for (int i = p1.x + match_type; i != p2.x + match_type; i+=match_type) {
            shells[i-match_type][p1.y] = shells[i][p1.y];
            shells[i][p1.y] = type1;
        }
    } else if (p1.x == p2.x) {
        int match_type = ((p2.y - p1.y) > 0) ? 1 : -1;
        for (int i = p1.y + match_type; i != p2.y + match_type; i+=match_type) {
            shells[p1.x][i-match_type] = shells[p1.x][i];
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

int Board::getCount(ShellType shell) {
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

bool Board::shellsMatch(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2, SDL_Point p3) {
    // Return false if any of the points are not in bounds
    if (!isWithinBounds(p1) || !isWithinBounds(p2) || !isWithinBounds(p3)) {
        return false;
    }

    return (
        shells[p1.x][p1.y] == shells[p2.x][p2.y] &&
        shells[p1.x][p1.y] == shells[p3.x][p3.y]
    );
}

unsigned int Board::rand() {
    this->seed = this->seed * 1664525 + 1013904223;
    return this->seed >> 24;
}

std::vector<std::vector<ShellType>> Board::getShellsCopy() {
    std::vector<std::vector<ShellType>> copy;
    copy.reserve(getWidth());
    for (int x = 0; x < getWidth(); x++) {
        copy.push_back(std::move(std::vector<ShellType>()));
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
