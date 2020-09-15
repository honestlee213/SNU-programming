// Knight's Tour Problem solving
// Created by Sehwan Lee(2016-19467) on 2020-09-12. Last updated 2020-09-15 Version.11
// Introduction to Data Structures 1st HomeWork.

// For those who are curious about Knight's Tour Problem, see this: http://en.wikipedia.org/wiki/Knight%27s_tour
// This program adopted Warnsdorff's algorithm.
// Also it has two different huristics to tie-break. Watch carefully how those are different.

#include <iostream>
#include <iomanip>      // For using setw(): implements "TAB"
#define N 8             // Size of boardboard
int board[N][N];        // Making boardboard in advance
using namespace std;

// This class makes objects Knight
// It has two member variables: row, col.
// And it has five member functions: isValid, countWarnsdorffNum, lastValue, simpleTour, complexTour.
// It deals the position where the knight is in.
class Knight {
    int row, col;       // 2-D array member variables
public:
    Knight(int i=0, int j=0) {      // constructor of Knight
        row = i;
        col = j;
    }
    ~Knight() {}                    // destructor
    // List of member functions; TBD
    bool isValid();
    int countWarnsdorffNum(int movingIdx, int *rowMove, int *colMove);
    void lastValue(int count, int *rowMove, int *colMove);
    void simpleTour(int *rowMove, int *colMove);
    void complexTour(int *rowMove, int *colMove);
};

// It makes format to print out Tour results.
// Only one function that is not member function.
// To tidy numbers up, use setw() option.
// Input: whether the tour was succeeded or not, no output.
void dispBoard(bool isSucceed) {
    // Print out format
    cout << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << setw(2) << board[i][j] << " ";      // Two seats for number, and a blank follows
        cout << endl;
    }

    // Print out whether the Tour was successful or not
    cout << endl;
    if (isSucceed)
        cout << "The tour was successful. " << endl;
    else
        cout << "The tour was unsuccessful. " << endl;
}

// It judges whether the cell in order to be occupied by Knight is valid to be filled in or not.
// 3 conditions to be valid: 0<=row<N, 0<=col<N, board[row][col] is unoccupied yet.
// No input is needed except Knight. Output: validity of the cell.
bool Knight::isValid() {
    return ((this->row >= 0) && (this->row < N) && (this->col >= 0) && (this->col < N) && (board[this->row][this->col] == 0));
}

// It implements Warnsdorff's rule.
// Once a cell is determined to be a target place to implement,
// checks its validity, and calculates the number of mobility.
// Inputs: Knight, direction to move(movingIdx), and two addresses of map(rowMove, colMove).
// Output: The sum of its mobility. If the cell is not valid or has no mobility, it returns zero.
int Knight::countWarnsdorffNum(int movingIdx, int *rowMove, int *colMove) {
    Knight temp1 = Knight(this->row + rowMove[movingIdx], this->col + colMove[movingIdx]);

    if (temp1.isValid()) {
        Knight temp2;
        int ableCount = 0;      // Sum of the cell's mobility
        // seekingIndex: iteration of 8 directions.
        for (int seekingIdx = 0; seekingIdx < 8; seekingIdx++) {
            temp2.row = temp1.row + rowMove[seekingIdx];
            temp2.col = temp1.col + colMove[seekingIdx];
            if (temp2.isValid())
                ableCount++;
        }

        return ableCount;
    }
    else
        return 0;
}

// It operates only to fill in the last value.
// For example, when in success condition, the last value N*N could not be filled in with countWarnsdorffNum().
// First, finds the position needs to fill the last value. Then fill it the count iteration got from two Tours.
// Inputs: count iteration, and two addresses of map(rowMove, colMove). No output.
void Knight::lastValue(int count, int *rowMove, int *colMove) {
    // Loop for finding the position to be fill in
    for (int seekingIdx = 0; seekingIdx < 8; seekingIdx++) {
        if (board[this->row + rowMove[seekingIdx]][this->col + colMove[seekingIdx]] == 0) {
            // Same as Tour()
            this->row += rowMove[seekingIdx];
            this->col += colMove[seekingIdx];
            board[this->row][this->col] = count;
            break;
        }
    }
}

// It is simpleTour tie-breaking algorithm.
// It calculates the Warnsdorff number of 8 neighbors, and moves cellNow to point which has smallest Warnsdorff number.
// If ties, chooses automatically. From the direction 10'O clock, finds towards clockwise.
// Inputs: initial point of object, two addresses of map(rowMove, colMove). No output.
void Knight::simpleTour(int *rowMove, int *colMove) {
    Knight tempKnight = Knight(this->row, this->col);   // Stores initial point and changes instantly
    int count;                                          // Counts iteration
    board[tempKnight.row][tempKnight.col] = 1;          // Starting point gets 0.

    for (count = 2; count < N*N; count++) {
        int minMoving = 8;          // Initialize minimum moving times to 8, because it is the maximum.
        int minMovingIdx = 0;       // Stores movingIndex of minMoving timing

        // Loops for movable 8 directions
        for (int movingIdx = 0; movingIdx < 8; movingIdx++) {
            int index = tempKnight.countWarnsdorffNum(movingIdx, rowMove, colMove);    // To shorten
            // Not operates Warnsdorff Number == 0 because it means dead-end.
            // Checks the direction makes minimum Warnsdorff number
            if ((index > 0) && (index < minMoving)) {
                minMoving = index;
                minMovingIdx = movingIdx;
            }
        }
        // Breaks loop when minMoving==8, because it means there's no minMoving moment. i.e. failure condition
        if (minMoving == 8)
            break;

        // After an iteration, updates cellNow and makes each point the number of counts.
        tempKnight.row += rowMove[minMovingIdx];
        tempKnight.col += colMove[minMovingIdx];
        board[tempKnight.row][tempKnight.col] = count;
    }

    tempKnight.lastValue(count, rowMove, colMove);    // Needs to fill lastValue because the upper loop does not fill it
    // When the SimpleTour succeed
    if (count == N*N)
        dispBoard(true);        // Print out the success condition
    else
        dispBoard(false);       // Print out the failure condition
}

// It is complexTour tie-breaking algorithm.
// It chooses the direction which has the smallest sum of Warnsdorff number.
// So it finds valid neighbors, and calculates sum of the Warnsdorff number of its neighbors.
// And moves cellNow to finding point.
// If ties, chooses automatically. From the direction 10'O clock, finds towards clockwise.
// Inputs: initial point of object, two addresses of map(rowMove, colMove). No output.
void Knight::complexTour(int *rowMove, int *colMove) {
    // Initialize boardboard again
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            board[i][j] = 0;
    }

    Knight tempKnight1 = Knight(this->row, this->col);  // Stores initial point and changes instantly
    Knight tempKnight2;                                 // Also is
    int count;                                          // Counts iteration
    board[tempKnight1.row][tempKnight1.col] = 1;        // Starting point gets 0.

    for (count = 2; count < N*N; count++) {
        int minMovingSum = 8*8;     // Initialize minimum moving sum to 8*8, because it is the maximum.
        int minMovingIdx = 0;       // Stores minimumMoving Index of minMovingSum timing

        // Loops for movable 8 directions
        for (int movingIdx = 0; movingIdx < 8; movingIdx++) {
            // Moves to point seeking sum of neighbors' Warnsdorff numbers
            tempKnight2.row = tempKnight1.row + rowMove[movingIdx];
            tempKnight2.col = tempKnight1.col + colMove[movingIdx];
            // When the next point is not valid, pass the loop index
            if (!tempKnight2.isValid())
                continue;

            int movingSum = 0;      // initialize moving times sum
            // Loops for summation of neighbors' Warnsdorff numbers and to find minimum timing
            for (int seekingIdx = 0; seekingIdx < 8; seekingIdx++)
                movingSum += tempKnight2.countWarnsdorffNum(seekingIdx, rowMove, colMove);
            // Not operates Warnsdorff Number == 0 because it means dead-end.
            // Checks the direction which makes minimum Warnsdorff number
            if ((movingSum > 0) && (movingSum < minMovingSum)) {
                minMovingSum = movingSum;
                minMovingIdx = movingIdx;
            }
        }
        // Breaks loop when no minMovingSum moment. i.e. failure condition
        if (minMovingSum == 8*8)
            break;

        // After an iteration, updates cellNow and makes each point the number of counts.
        tempKnight1.row += rowMove[minMovingIdx];
        tempKnight1.col += colMove[minMovingIdx];
        board[tempKnight1.row][tempKnight1.col] = count;
    }

    tempKnight1.lastValue(count, rowMove, colMove);     // Fills lastValue
    // When the ComplexTour succeed
    if (count == N*N)
        dispBoard(true);        // Print out the success condition
    else
        dispBoard(false);       // Print out the failure condition
}

// The main starts entire program.
// It shows information of this program.
// Also receives starting point, and triggers two tour function algorithms: simpleTour() and complexTour().
// No input, no output except myKnight. Trivially.
int main() {
    // Chessboard initialization
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            board[i][j] = 0;
    }
    int rowInput, colInput;       // To store user's input point
    int rowMove[8] = {-1, -2, -2, -1, 1, 2, 2, 1};      // Map of row moving. It contains directions in row.
    int colMove[8] = {-2, -1, 1, 2, 2, 1, -1, -2};      // Map of column moving. It contains directions in row.

    // Shows program information
    cout << "The Knight's Tour Problem solving program" << endl;
    cout << "Made by Sehwan Lee(2016-19467). " << endl;
    cout << "Input i j of the starting point (i, j) between 1 and N. " << endl;
    cout << "For example: Type 4 4 if you want to input (4, 4). " << endl;
    cout << "(i, j): ";
    cin >> rowInput >> colInput;

    Knight myKnight = Knight(rowInput-1, colInput-1);       // To make chessboard range between 1 and 8.

    // Check whether inputs is in range between 0 and N.
    if (!myKnight.isValid()) {
        cout << "The inputs are in invalid range. " << endl;
        cout << "Return to first page. " << endl;
        return 0;        // if not, finishes program
    }
    else {
        cout << "The simple tour has just begun. " << endl;
        myKnight.simpleTour(rowMove, colMove);     // Implements simpleTour first
        cout << "The simple tour has just ended. Now the complex tour will follow. " << endl;
        myKnight.complexTour(rowMove, colMove);    // Implements complexTour immediately after simpleTour finishes
        cout << "All tours has just ended. " << endl;

        return 0;   // End of entire program
    }
}