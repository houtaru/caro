#ifndef DATA_H_DEFINED
#define DATA_H_DEFINED

/** saved*.game format
 * flag (0 : do not exist; 1 : exist)
 * type (0 : PVP; 1 : PVC)
 * m n  (m : number of rows; n : number of cols)
 * M lines: each line n numbers. Represent the gameboard of the match.
 *      -> 1 : first player; 
 *      -> 2 : second player/machine; 
 *      -> 0 : no one do in this cell.
 * */

/** *.statis format
 * Consist of number of lines. 
 * Each line has: yyyy.mm.dd hh:mm:ss m n state
 * 
 * Note:
 *  + yyyy.mm.dd : year.month.day of the play;.
 *  + hh:mm:ss time when end the game.
 *  + m, n: length of lines and cols of the state.
 *  + state: game board of the match
 * */

namespace Data {
    void savedGame(bool flag = true);

    bool canLoadGame(int flag); // 0 : PVP; 1 : PVC

    void loadGame(int flag); // 0 : PVP; 1 : PVC

    void gameStatis(int flag); // 0 : PVP; 1 : PVC

    void loadStatis(int flag); // 0 : PVP; 1 : PVC
};

#endif