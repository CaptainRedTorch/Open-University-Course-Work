package mmn.mmn12;

import java.util.Random;

public class GameLogic {

    //The Game of Life logic class

    final Random rand = new Random();

    private final int boardSize;    // Dimension of the board
    private int genCount=0;         // Counts the generations
    private boolean[][] board;      // The Board containing the tiles, true = alive, false = dead

    /**
     * Creates a new Game of Life game.
     * @param boardSize dimension of the board
     * @param isRandom  will initialize a random board if true
     */
    public GameLogic(int boardSize, boolean isRandom) {
        this.boardSize = boardSize;
        board = new boolean[boardSize][boardSize];
        //generate a random board
        if (isRandom) {
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    board[i][j] = rand.nextBoolean();
                }
            }
        }
    }

    /**
     * @return dimension of board
     */
    public int getBoardSize(){
        return boardSize;
    }

    /**
     * @param x length
     * @param y height
     * @return The value of the tile at board[x,y]
     */
    public boolean getBoardTile(int x, int y){
        if (x<0 || x>=boardSize || y<0 || y>=boardSize)
            return false;
        return board[x][y];
    }

    /**
     * Set the tile at [x,y]
     * @param x length
     * @param y height
     * @param tile true: alive, false: dead
     */
    public void setBoardTile(int x, int y, boolean tile){
        board[x][y] = tile;
    }

    /**
     * @return The current generation count
     */
    public int getGenCount() {
        return genCount;
    }

    /**
     * Generation count ++
     */
    public void addGenCount() {
        genCount++;
    }

    /**
     * Calculates the next generation of the current game
     * and updates the board
     */
    public void nextGen(){
        //initialize a temp board to calculate next gen onto
        GameLogic nextGen = new GameLogic(boardSize,false);
        int neighbors;
        final int neighborsForBirth = 3;
        final int neighborsForPersist = 2;

        //check each tile
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {

                //count neighbors
                neighbors = 0 ;
                neighbors += this.getBoardTile(i-1,j-1)? 1:0;
                neighbors += this.getBoardTile(i,j-1)? 1:0;
                neighbors += this.getBoardTile(i+1,j-1)? 1:0;
                neighbors += this.getBoardTile(i-1,j)? 1:0;
                neighbors += this.getBoardTile(i+1,j)? 1:0;
                neighbors += this.getBoardTile(i-1,j+1)? 1:0;
                neighbors += this.getBoardTile(i,j+1)? 1:0;
                neighbors += this.getBoardTile(i+1,j+1)? 1:0;

                //create next generation based on count
                if (this.getBoardTile(i, j)) {
                    //death
                    if (neighbors < neighborsForPersist || neighbors > neighborsForBirth)
                        nextGen.setBoardTile(i, j, false);
                    //persist
                    else
                        nextGen.setBoardTile(i, j, true);
                }
                //birth
                else if (neighbors == neighborsForBirth)
                    nextGen.setBoardTile(i, j, true);

            }
        }
        //copy the new gen onto the board
        this.board = nextGen.board;
    }


}
