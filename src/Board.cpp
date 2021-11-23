#include "Board.hpp"

Board::Board(int width, int height) {
   // Create the gems vector
   this->gems.reserve(width);
    for (int x = 0; x < width; x++) {
        this->gems.push_back(std::move(std::vector<Gem>()));
        this->gems[x].reserve(height);
        for (int y = 0; y < height; y++) {
            this->gems[x].push_back(Gem::EMPTY);
        }
    }

    // Fill the array and remove matches
    while(hasEmpty()) {
        fillEmpty();
        match();
    }
}

int Board::match() {
    std::vector<SDL_Point> to_remove;
    for (int x = 0; x < getWidth(); x++) {
        Gem last = Gem::EMPTY;
        int matches = 0;
        for (int y = 0; y < getHeight(); y++) {
            if (this->gems[x][y] == last) {
                if (last != Gem::EMPTY) {
                    matches++;
                } else {
                    matches = 0;
                    continue;
                }
                if (y == (getHeight() - 1) && matches > 1) {
                    for (; matches >= 0; matches--) {
                        to_remove.push_back(SDL_Point{x,y-matches});
                    }
                }
            } else {
                if (matches > 1) {
                    for (; matches >= 0; matches--) {
                        to_remove.push_back(SDL_Point{x,y-1-matches});
                    }
                }
            }
            last = this->gems[x][y];
        }
    }

    for (int y = 0; y < getHeight(); y++) {
        Gem last = Gem::EMPTY;
        int matches = 0;
        for (int x = 0; x < getWidth(); x++) {
            if (this->gems[x][y] == last) {
                if (last != Gem::EMPTY)
                    matches++;
                if (x == (getWidth() - 1) && matches > 1) {
                    for (; matches >= 0; matches--) {
                        to_remove.push_back(SDL_Point{x,y-matches});
                    }
                    matches = 0;
                }
            } else {
                if (matches > 1) {
                    for (; matches >= 0; matches--) {
                        to_remove.push_back(SDL_Point{x-1-matches,y});
                    }
                }
                matches = 0;
            }
            last = this->gems[x][y];
        }
    }
    for (SDL_Point p : to_remove) {
        this->gems[p.x][p.y] = Gem::EMPTY;
    }
    
    return to_remove.size();
}

void Board::fillEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = (getHeight() - 1); y >= 0; y--) {
            if (this->gems[x][y] != Gem::EMPTY)
                continue;
            
            if (y > 0) {
                this->gems[x][y] = this->gems[x][y-1];
                this->gems[x][y-1] = Gem::EMPTY;
            } else {
                this->gems[x][y] = (Gem) (rand() % (int) Gem::NUMBER_OF_COLORS);
            }
        }
    }
}

bool Board::hasEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            if (this->gems[x][y] == Gem::EMPTY) {
                return true;
            }
        }
    }
    return false;
}

bool Board::swap(SDL_Point p1, SDL_Point p2) { 
    if (!isWithinBounds(p1) || !isWithinBounds(p2)) {
        return false;
    }
    
    bool shouldSwap = false;

    Gem type1 = this->gems[p1.x][p1.y];
    Gem type2 = this->gems[p2.x][p2.y];

    
    if (typeWouldMatchOnPoint(type1, p2)) {
        shouldSwap = true;
    } else if (typeWouldMatchOnPoint(type2, p1)) {
        shouldSwap = true;
    }

    if (shouldSwap) {
        this->gems[p1.x][p1.y] = type2;
        this->gems[p2.x][p2.y] = type1;
    }

    return shouldSwap;
}

bool Board::typeWouldMatchOnPoint(Gem type, SDL_Point point) {
    // Check horizontal
    int matchingGems = 0;
    for (int x = std::max(0, (point.x - 2)); x < std::min(getWidth(), point.x + 3); x++) {
        if (x == point.x) {
            matchingGems++;
        } else if (this->gems[x][point.y] == type) {
            matchingGems++;
        } else if (x < point.x){
            matchingGems = 0;
        } else {
            break;
        }
    }
    if (matchingGems > 2) {
        return true;
    }

    // Check vertical
    matchingGems = 0;
    for (int y = std::max(0, (point.y - 2)); y < std::min(getHeight(), point.y + 3); y++) {
        if (y == point.y) {
            matchingGems++;
            continue;
        }
        if (this->gems[point.x][y] == type) {
            matchingGems++;
        } else if (y < point.y){
            matchingGems = 0;
        } else {
            break;
        }
    }
    if (matchingGems > 2) {
        return true;
    }

    return false;
}


bool Board::isWithinBounds(SDL_Point point) {
    bool result = true;
    if (point.x < 0 || point.x > (getWidth() - 1)) {
        SDL_Log("Point %d,%d is not on the board.", point.x, point.y);
        result = false;
    }
    if (point.y < 0 || point.y > (getHeight() - 1)) {
        SDL_Log("Point %d,%d is not on the board.", point.x, point.y);
        result = false;
    }
    return result;
}

int Board::getHeight() {
    return gems[0].size();
}

int Board::getWidth() {
    return gems.size();
}
