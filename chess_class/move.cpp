#include "chess.h"

U64 chess::knightAttacks(U64 knights) {
    U64 l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7f;
    U64 l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3f;
    U64 r1 = (knights << 1) & 0xfefefefefefefefe;
    U64 r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfc;
    U64 h1 = l1 | r1;
    U64 h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
};

bool chess::addPiece(int piece, int target) {

    U64 init = 0;
    U64& pKnight = init;
    U64& pBeshop = init;
    U64& pRook = init;
    U64& pQueen = init;
    U64 addtarget = (1ULL << target);

    if (turn == Turn::white) {
        pKnight = WhiteKnightField;
        pBeshop = WhiteBeshopField;
        pRook = WhiteRookField;
        pQueen = WhiteQueenField;
    }
    else if (turn == Turn::black) {
        pKnight = BlackKnightField;
        pBeshop = BlackBeshopField;
        pRook = BlackRookField;
        pQueen = BlackQueenField;
    }

    switch (piece) {
    case 1:
        pKnight = pKnight + addtarget;
        break;
    case 2:
        pBeshop = pBeshop + addtarget;
        break;
    case 3:
        pRook = pRook + addtarget;
        break;
    case 4:
        pQueen = pQueen + addtarget;
        break;
    default:
        return false;
    }
    return true;
}
void chess::AttackAndDelete(IO_info* info) {
    if (turn == Turn::white) {
        if ((BlackPawnField & (1ULL << info->move)) != 0) {
            BlackPawnField = BlackPawnField - (1ULL << info->move);
        }
        else if ((BlackKnightField & (1ULL << info->move)) != 0) {
            BlackKnightField = BlackKingField - (1ULL << info->move);
        }
        else if ((BlackBeshopField & (1ULL << info->move)) != 0) {
            BlackBeshopField = BlackBeshopField - (1ULL << info->move);
        }
        else if ((BlackRookField & (1ULL << info->move)) != 0) {
            BlackRookField = BlackRookField - (1ULL << info->move);
        }
        else if ((BlackQueenField & (1ULL << info->move)) != 0) {
            BlackQueenField = BlackQueenField - (1ULL << info->move);
        }

    }
    else if (turn == Turn::black) {
        if ((WhitePawnField & (1ULL << info->move)) != 0) {
            WhitePawnField = WhitePawnField - (1ULL << info->move);
        }
        else if ((WhiteKnightField & (1ULL << info->move)) != 0) {
            WhiteKnightField = WhiteKingField - (1ULL << info->move);
        }
        else if ((WhiteBeshopField & (1ULL << info->move)) != 0) {
            WhiteBeshopField = WhiteBeshopField - (1ULL << info->move);
        }
        else if ((WhiteRookField & (1ULL << info->move)) != 0) {
            WhiteRookField = WhiteRookField - (1ULL << info->move);
        }
        else if ((WhiteQueenField & (1ULL << info->move)) != 0) {
            WhiteQueenField = WhiteQueenField - (1ULL << info->move);
        }

    }
 
}

U64 PawnAttackMap(int color, int target) {
    U64 returnMap = 0;
    if (color == Turn::white) {
        if (target >= 56) {
            return 0;
        }
        returnMap = returnMap | (1ULL << (target + 8));
        if (target + 16 < 64) {
            returnMap = returnMap | (1ULL << (target + 16));
        }
        if ((target) % 8 != 0) {
            returnMap = returnMap | (1ULL << (target + 7));
        }
        if ((target) % 8 != 7) {
            returnMap = returnMap | (1ULL << (target + 9));
        }
    }
    else if (color == Turn::black) {
        if (target < 8) {
            return 0;
        }
        returnMap = returnMap | (1ULL << (target - 8));
        if (target - 16 > 0) {
            returnMap = returnMap | (1ULL << (target - 16));
        }
        if ((target) % 8 != 7) {
            returnMap = returnMap | (1ULL << (target - 7));
        }
        if ((target) % 8 != 0) {
            returnMap = returnMap | (1ULL << (target - 9));
        }
    }
    return returnMap;
}

bool chess::movePawn(IO_info *info) {
    int TurnTemp = 0;
    U64 CompareValue = 0;
    U64 CompareValue2 = 0;
    U64 CheckAttack = 0;
    U64 *MyPieceTemp = 0;
    U64 *EnemyPieceTemp = 0;
    U64 *TargetPawnMap = 0;


    CheckAttack = ::PawnAttackMap(turn, info->target);
    if ((CheckAttack & (1ULL << info->move)) == 0) {
        std::cout << "don't move pawn" << std::endl;
        return false;
    }

    if (turn == Turn::white) { 
        TurnTemp = 1; 
        CompareValue = 0xff00;
        MyPieceTemp = &WhiteMap;
        EnemyPieceTemp = &BlackMap;
        TargetPawnMap = &WhitePawnField;
    }
    else if (turn == Turn::black) { 
        TurnTemp = -1;
        CompareValue = 0xFF000000000000;
        MyPieceTemp = &BlackMap;
        EnemyPieceTemp = &WhiteMap;
        TargetPawnMap = &BlackPawnField;
    }
    else { std::cout << "error" << std::endl; return false; }
    
    //������ ��ĭ �Ǵ� ��ĭ ����
    if (info->target == info->move - (8* TurnTemp)) {
        //�տ� �ڽ��� ���� �ִ°�? �Ǵ� �ڽ��� ���� �������� ���� �ִ°�?
        if ((*MyPieceTemp & (1ULL << info->move)) != 0) {
            std::cout << "error: pawn blocked by same color piece" << '\n';
            delete info;
            return false;
        }
        //����� �⹰�� ���� ��������� �ϴ� ��θ� ���θ��� �ִ°�?
        else if ((*EnemyPieceTemp & (1ULL << info->move)) != 0) {
            std::cout << "error: pawn blocked by same color piece " << '\n';
            delete info;
            return false;
        }
    }
    else if (info->target == info->move - (16 * TurnTemp)) {
        U64 temp = (1ULL << (info->target + (8 * TurnTemp))) | (1ULL << (info->target + (16 * TurnTemp)));
        //���� ��ĭ���� ������ ĭ�� �ִ°�?
        if ((CompareValue & (1ULL << info->target)) == 0) {
            std::cout << "error: pawn doesn't double move" << '\n';
            delete info;
            return false;
        }
        //�տ� �ڽ��� ���� �ִ°�? �Ǵ� �ڽ��� ���� �������� ���� �ִ°�?
        if ((*MyPieceTemp & temp) != 0) {
            std::cout << "error: pawn blocked by same color piece" << '\n';
            delete info;
            return false;
        }
        //����� �⹰�� ���� ��������� �ϴ� ��θ� ���θ��� �ִ°�?
        else if ((*EnemyPieceTemp & temp) != 0) {
            std::cout << "error: pawn blocked by same color piece " << '\n';
            delete info;
            return false;
        }
        //���Ļ� ������
        enpassant = info->move;
    }

    //���� �밢������ ������
    else if (info->target == info->move - (7 * TurnTemp) || info->target == info->move - (9 * TurnTemp)) {
        //�Ǽ��� �ڽ��� ���� �����ߴ°�?

        if ((*MyPieceTemp & (1ULL << info->move)) != 0) {
            std::cout << "error:same color piece attack" << '\n';
            delete info;
            return false;
        }
        //���ݰ����� ����� ���� ���� �ȿ� �ִ°�?
        else if ((*EnemyPieceTemp & (1ULL << info->move)) == 0 && enpassant == square::none) {
            std::cout << "error:Enemy Piece Doesn't exist" << '\n';
            delete info;
            return false;
        }

        //�������� ����� �⹰ ����
        if (enpassant == square::none) {
           AttackAndDelete(info);

        }
        else if (enpassant != square::none) {
            if (turn = Turn::white) {
                BlackPawnField = BlackPawnField - (1ULL << enpassant);
            }
            else if (turn = Turn::black) {
                WhitePawnField = WhitePawnField - (1ULL << enpassant);
            }
            enpassant = square::none;
        }
        //���� ���� �����ߴ°�?
        if (info->move >= a8 || info->move <= h1) {
            int change;
            std::cout << "piece(1.knight, 2.beshop, 3.rook, 4.queen):";
            std::cin >> change;
            addPiece(change, info->move);
            *TargetPawnMap = *TargetPawnMap - (1ULL << info->target);
            delete info;
            return true;
        }

    }
 
    *TargetPawnMap = *TargetPawnMap - (1ULL << info->target);
    *TargetPawnMap = *TargetPawnMap | (1ULL << info->move);
    delete info;
    return true;
}

bool chess::moveKnight(IO_info* info) {

    U64 init = 0;
    U64* MyPiecetMap = &init;
    U64* EnemyPiecetMap = &init;
    U64* TargetKnightMap = &init;
    U64 attackpos = (1ULL << info->move);

    if (info->target == square::none) {
        std::cout << "error: Wrong ACN" << std::endl;
        delete info;
        return false;
    }

    if (turn == Turn::white) {
        MyPiecetMap = &WhiteMap;
        EnemyPiecetMap = &BlackMap;
        TargetKnightMap = &WhiteKnightField;
    }
    else if (turn == Turn::black) {
        MyPiecetMap = &BlackMap;
        EnemyPiecetMap = &WhiteMap;
        TargetKnightMap = &BlackKnightField;
    }

    //�� �⹰�� �����ߴ°�?
    if ((*MyPiecetMap & attackpos) != 0) {
        std::cout << "error:My Piece Attack" << '\n';
        delete info;
        return false;
    }

    //��� �⹰�� �����ߴ°�?
    if ((*EnemyPiecetMap & attackpos) != 0) {
       AttackAndDelete(info);
    }

    *TargetKnightMap = *TargetKnightMap - (1ULL << info->target);
    *TargetKnightMap = *TargetKnightMap | (1ULL << info->move);
    delete info;
   
    return true;
}

bool chess::moveBeshop(IO_info* info) {
    U64 init = 0;
    U64* MyPiecetMap = &init;
    U64* EnemyPiecetMap = &init;
    U64* TargetBeshopMap = &init;
    U64 attackpos = (1ULL << info->move);

    if (turn == Turn::white) {
        MyPiecetMap = &WhiteMap;
        EnemyPiecetMap = &BlackMap;
        TargetBeshopMap = &WhiteBeshopField;
    }
    else if (turn == Turn::black) {
        MyPiecetMap = &BlackMap;
        EnemyPiecetMap = &WhiteMap;
        TargetBeshopMap = &BlackBeshopField;
    }

    else if (info->target == square::none) {
        std::cout << "error: Wrong ACN" << std::endl;
        delete info;
        return false;
    }

    //�ڽ��� �⹰�� �����ߴ°�?
    if ((*MyPiecetMap & (1ULL << info->move)) != 0) {
        std::cout << "error:My Piece Attack" << '\n';
        delete info;
        return false;
    }
    //����� �⹰�� �����ߴ°�?
    if ((*EnemyPiecetMap & attackpos) != 0) {
        AttackAndDelete(info);
    }

    *TargetBeshopMap = *TargetBeshopMap - (1ULL << info->target);
    *TargetBeshopMap = *TargetBeshopMap | (1ULL << info->move);
    delete info;

    return true;

}
bool chess::moveRook(IO_info* info) {
    U64 init = 0;
    U64* MyPiecetMap = &init;
    U64* EnemyPiecetMap = &init;
    U64* TargetRookMap = &init;
    U64 attackpos = (1ULL << info->move);

    if (turn == Turn::white) {
        MyPiecetMap = &WhiteMap;
        EnemyPiecetMap = &BlackMap;
        TargetRookMap = &WhiteRookField;
    }
    else if (turn == Turn::black) {
        MyPiecetMap = &BlackMap;
        EnemyPiecetMap = &WhiteMap;
        TargetRookMap = &BlackRookField;
    }

    //ĳ���� �Ͽ��°�?
    if (info->casling == queen_side) {
        if(turn == Turn::white){
            //��� ���� �� �ڸ��� �ִ°�?
            U64 compareValue = 144;
            U64 CompareValue2 = 96;
            if((WhiteRookField & compareValue) == 0 &&
                (WhiteKingField & compareValue) == 0){ 
                std::cout << "error: doesn't casling" << std::endl;
                delete info;
                return false;
            }
            //��� �� ���̿� �ƹ� �⹰�� �������� �ʴ°�?
            else if ((bitmap & CompareValue2) != 0) {
                std::cout << "error: doesn't casling, Piece exist" << std::endl;
                delete info;
                return false;
            }

            //��� ���� �ѹ��̶� �������°�?
            else if (WLRookFlag == false || WKingMoveFlag == false) {
                std::cout << "error: doesn't casling, Once move" << std::endl;
                delete info;
                return false;
            }

            WhiteRookField = WhiteRookField - 128 + 32;
            WhiteKingField = 64;
            WLRookFlag = false;
            WRRookFlag = false;
            WKingMoveFlag = false;
            delete info;
            
        }
        else if (turn == Turn::black) {
            //��� ���� �� �ڸ��� �ִ°�?
            U64 compareValue = 0x9000000000000000;
            U64 CompareValue2 = 0x6000000000000000;
            if ((BlackRookField & compareValue) == 0 &&
                (BlackKingField & compareValue) == 0) {
                std::cout << "error: doesn't casling" << std::endl;
                delete info;
                return false;
            }
            //��� �� ���̿� �ƹ� �⹰�� �������� �ʴ°�?
            else if ((bitmap & CompareValue2) != 0) {
                std::cout << "error: doesn't casling, Piece exist" << std::endl;
                delete info;
                return false;
            }

            //��� ���� �ѹ��̶� �������°�?
            else if (BLRookFlag == false || BKingMoveFlag == false) {
                std::cout << "error: doesn't casling, Once move" << std::endl;
                delete info;
                return false;
            }

            BlackRookField = BlackRookField - 0x8000000000000000 + 0x2000000000000000;
            BlackKingField = 0x4000000000000000;
            BLRookFlag = false;
            BRRookFlag = false;
            BKingMoveFlag = false;
            delete info;
        }
        return true;
    }

    else if (info->casling == king_side) {
        if (turn == Turn::white) {
            //��� ���� �� �ڸ��� �ִ°�?
            U64 compareValue = 17;
            U64 CompareValue2 = 14;
            if ((WhiteRookField & compareValue) == 0 &&
                (WhiteKingField & compareValue) == 0) {
                std::cout << "error: doesn't casling" << std::endl;
                delete info;
                return false;
            }
            //��� �� ���̿� �ƹ� �⹰�� �������� �ʴ°�?
            else if ((bitmap & CompareValue2) != 0) {
                std::cout << "error: doesn't casling, Piece exist" << std::endl;
                delete info;
                return false;
            }

            //��� ���� �ѹ��̶� �������°�?
            else if (WRRookFlag == false || WKingMoveFlag == false) {
                std::cout << "error: doesn't casling, Once move" << std::endl;
                delete info;
                return false;
            }

            WhiteRookField = WhiteRookField -1 + 8;
            WhiteKingField = 2;
            WLRookFlag = false;
            WRRookFlag = false;
            WKingMoveFlag = false;
            delete info;
        }
        else if (turn == Turn::black) {
            //��� ���� �� �ڸ��� �ִ°�?
            U64 compareValue = 0x8800000000000000;
            U64 CompareValue2 = 0x7000000000000000;
            if ((BlackRookField & compareValue) == 0 &&
                (BlackKingField & compareValue) == 0) {
                std::cout << "error: doesn't casling" << std::endl;
                delete info;
                return false;
            }
            //��� �� ���̿� �ƹ� �⹰�� �������� �ʴ°�?
            else if ((bitmap & CompareValue2) != 0) {
                std::cout << "error: doesn't casling, Piece exist" << std::endl;
                delete info;
                return false;
            }

            //��� ���� �ѹ��̶� �������°�?
            else if (BRRookFlag == false || BKingMoveFlag == false) {
                std::cout << "error: doesn't casling, Once move" << std::endl;
                delete info;
                return false;
            }

            BlackRookField = BlackRookField - 0x100000000000000 + 0x800000000000000;
            BlackKingField = 0x200000000000000;
            BLRookFlag = false;
            BRRookFlag = false;
            BKingMoveFlag = false;
            delete info;
        }
        return true;
    }

    else if (info->target == square::none) {
        std::cout << "error: Wrong ACN" << std::endl;
        delete info;
        return false;
    }

    //�ڽ��� �⹰�� �����ߴ°�?
    if ((*MyPiecetMap & (1ULL << info->move)) != 0) {
        std::cout << "error:My Piece Attack" << '\n';
        delete info;
        return false;
    }

    //����� �⹰�� �����ߴ°�?
    if ((*EnemyPiecetMap & attackpos) != 0) {
        AttackAndDelete(info);
    }

    //ó������ ���� ������°�?
    if (info->target == a1 && WLRookFlag == true) {
        WLRookFlag = false;
    }
    else if (info->target == h1 && WRRookFlag == true) {
        WRRookFlag = false;
    }
    else if (info->target == a8 && BLRookFlag == true) {
        BLRookFlag == false;
    }
    else if (info->target == h8 && BRRookFlag == true) {
        BRRookFlag == false;
    }

    *TargetRookMap = *TargetRookMap - (1ULL << info->target);
    *TargetRookMap = *TargetRookMap | (1ULL << info->move);
    delete info;

    return true;
}

bool chess::moveQueen(IO_info* info) {
    U64 init = 0;
    U64* MyPiecetMap = &init;
    U64* EnemyPiecetMap = &init;
    U64* TargetQueenMap = &init;
    U64 attackpos = (1ULL << info->move);

    if (turn == Turn::white) {
        MyPiecetMap = &WhiteMap;
        EnemyPiecetMap = &BlackMap;
        TargetQueenMap = &WhiteQueenField;
    }
    else if (turn == Turn::black) {
        MyPiecetMap = &BlackMap;
        EnemyPiecetMap = &WhiteMap;
        TargetQueenMap = &BlackQueenField;
    }

    else if (info->target == square::none) {
        std::cout << "error: Wrong ACN" << std::endl;
        delete info;
        return false;
    }

    //�ڽ��� �⹰�� �����ߴ°�?
    if ((*MyPiecetMap & (1ULL << info->move)) != 0) {
        std::cout << "error:My Piece Attack" << '\n';
        delete info;
        return false;
    }

    //����� �⹰�� �����ߴ°�?
    if ((*EnemyPiecetMap & attackpos) != 0) {
        AttackAndDelete(info);
    }

    *TargetQueenMap = *TargetQueenMap - (1ULL << info->target);
    *TargetQueenMap = *TargetQueenMap | (1ULL << info->move);
    delete info;


    return true;
}
bool chess::moveKing(IO_info* info) {
    U64 pawnpos = 0;

    U64 init = 0;
    U64* MyPiecetMap = &init;
    U64* EnemyPiecetMap = &init;
    U64* EnemyPawnMap = &init;
    U64* EnemyKnightMap = &init;
    U64* EnemyRookMap = &init;
    U64* EnemyBeshopMap = &init;
    U64* EnemyQueenMap = &init;
    U64* EnemyKingMap = &init;
    U64* TargetKingMap = &init;
    U64 attackpos = (1ULL << info->move);
    if (turn == Turn::white) {
        pawnpos = (attackpos << 7) | (attackpos << 9);
        MyPiecetMap = &WhiteMap;
        EnemyPiecetMap = &BlackMap;
        EnemyPawnMap = &BlackPawnField;
        EnemyKnightMap = &BlackKnightField;
        EnemyRookMap = &BlackRookField;
        EnemyBeshopMap = &BlackBeshopField;
        EnemyQueenMap = &BlackQueenField;
        EnemyKingMap = &BlackKingField;
        TargetKingMap = &WhiteKingField;
    }
    else if (turn == Turn::black) {
        pawnpos = (attackpos >> 7) | (attackpos >> 9);
        MyPiecetMap = &BlackMap;
        EnemyPiecetMap = &WhiteMap;
        EnemyPawnMap = &WhitePawnField;
        EnemyKnightMap = &WhiteKnightField;
        EnemyRookMap = &WhiteRookField;
        EnemyBeshopMap = &WhiteBeshopField;
        EnemyQueenMap = &WhiteQueenField;
        EnemyKingMap = &WhiteKingField;
        TargetKingMap = &BlackKingField;
    }

    if ((*MyPiecetMap & attackpos) != 0) {
        std::cout << "error:My Piece Attack" << '\n';
        delete info;
        return false;
    }
    if ((pawnpos & *EnemyPawnMap) != 0){
        std::cout << "error: Enemy Pawn attack possition" << '\n';
        delete info;
        return false;
    }
    
    if ((knightAttacks(info->move) & *EnemyKnightMap) != 0) {
        std::cout << "error: Enemy Knight attack possition" << '\n';
        delete info;
        return false;
    }
    
    if ((BeshopMask[info->move] & *EnemyBeshopMap) != 0 || 
        (BeshopMask[info->move] &*EnemyQueenMap) != 0) {
        std::cout << "error: Enemy Beshop attack possition" << '\n';
        delete info;
        return false;
    }

    if ((RookMask[info->move] & *EnemyRookMap & *EnemyQueenMap) != 0
        (RookMask[info->move] & *EnemyQueenMap) != 0) {
        std::cout << "error: Enemy Rook attack possition" << '\n';
        delete info;
        return false;
    }
    if ((KingMoveMask((1ULL << info->move)) & *EnemyKingMap) != 0) {
        std::cout << "error: Enemy King attack possition" << '\n';
        delete info;
        return false;
    }

    if ((*EnemyPiecetMap & attackpos) != 0) {
         AttackAndDelete(info);
    }

    *TargetKingMap = *TargetKingMap - (1ULL << info->target);
    *TargetKingMap = *TargetKingMap | (1ULL << info->move);
    delete info;

    return true;
}

bool chess::Move(IO_info* info) {
    bool result = true;

    switch (info->piece) {
    case Piece::pawn:
        result = movePawn(info);
        break;
    case Piece::knight:
        result = moveKnight(info);
        break;
    case Piece::beshop:
        result = moveBeshop(info);
        break;
    case Piece::rook:
        result = moveRook(info);
        break;
    case Piece::queen:
        result = moveQueen(info);
        break;
    case Piece::king:
        result = moveKing(info);
        break;
    }

    return result;
}