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
    std::vector<SDL_Point> matchedGems;
    int matchesFound = getMatches(this->gems, &matchedGems);

    for (SDL_Point p : matchedGems) {
        this->gems[p.x][p.y] = Gem::EMPTY;
    }

    return matchesFound;
}

int Board::getMatches(std::vector<std::vector<Gem>> gems, std::vector<SDL_Point> * matchedGems=nullptr){
    int matchesFound = 0;

    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight() - 2; y++) {
            if (gems[x][y] == gems[x][y+1] && gems[x][y] == gems[x][y+2]) {
                if (matchedGems != nullptr) {
                    matchedGems->push_back({x,y});
                    matchedGems->push_back({x,y+1});
                    matchedGems->push_back({x,y+2});
                }
                matchesFound++;
            }
        }
    }
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth() - 2; x++) {
            if (gems[x][y] == gems[x+1][y] && gems[x][y] == gems[x+2][y]) {
                if (matchedGems != nullptr) {
                    matchedGems->push_back({x,y});
                    matchedGems->push_back({x+1,y});
                    matchedGems->push_back({x+2,y});
                }
                matchesFound++;
            }
        }
    }

    return matchesFound;
}

void Board::fillEmpty() {
    for (int x = 0; x < getWidth(); x++) {
        for (int y = (getHeight() - 1); y >= 0; y--) {
            if (this->gems[x][y] != Gem::EMPTY)
                continue;
            
            // Make the gem above us fall
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
    // Make sure the swapped gems are on the same line
    if (p1.x != p2.x && p1.y != p2.y) {
        return false;
    }

    // Make sure the positions are on the board
    if (!isWithinBounds(p1) || !isWithinBounds(p2)) {
        return false;
    }
    
    std::vector<std::vector<Gem>> swappedGems = getGemsAfterSwap(this->gems, p1, p2);
    if (getMatches(swappedGems) > 0) {
        this->gems = swappedGems;
        return true;
    } else {
        return false;
    }
}

std::vector<std::vector<Gem>> Board::getGemsAfterSwap(std::vector<std::vector<Gem>> gems, SDL_Point p1, SDL_Point p2) {
    Gem type1 = gems[p1.x][p1.y];
    if (p1.y == p2.y) {
        int direction = ((p2.x - p1.x) > 0) ? 1 : -1;
        for (int i = p1.x + direction; i != p2.x + direction; i+=direction) {
            gems[i-direction][p1.y] = gems[i][p1.y];
            gems[i][p1.y] = type1;
        }
    } else if (p1.x == p2.x) {
        int direction = ((p2.y - p1.y) > 0) ? 1 : -1;
        for (int i = p1.y + direction; i != p2.y + direction; i+=direction) {
            gems[p1.x][i-direction] = gems[p1.x][i];
            gems[p2.x][i] = type1;
        }
    }

    return gems;
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

std::vector<std::vector<Gem>> Board::getGemsCopy() {
    std::vector<std::vector<Gem>> copy;
    copy.reserve(getWidth());
    for (int x = 0; x < getWidth(); x++) {
        copy.push_back(std::move(std::vector<Gem>()));
        copy[x].reserve(getHeight());
        for (int y = 0; y < getHeight(); y++) {
            copy[x].push_back(this->gems[x][y]);
        }
    }
    return copy;
}

int Board::getHeight() {
    return gems[0].size();
}

int Board::getWidth() {
    return gems.size();
}
