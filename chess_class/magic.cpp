#include "chess.h"

/**************************************\
 ======================================

         Plain magic bitboards
     implementation & demonstration

                  by

           Code Monkey King

 ======================================
\**************************************/

// system headers
#include <cstdio>
#include <cstdlib>
#include <cstring>


// bits manipulations
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

// square encoding
// rook & bishop flags
enum { frook, fbishop };

// rook rellevant occupancy bits
int rook_rellevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// bishop rellevant occupancy bits
int bishop_rellevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// just a random number
unsigned int state = 1804289383;

// 32-bit number pseudo random generator
unsigned int generate_random_number()
{
    // XOR shift algorithm
    unsigned int x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

// generate random U64 number
U64 random_U64()
{
    // init numbers to randomize
    U64 u1, u2, u3, u4;

    // randomize numbers
    u1 = (U64)(generate_random_number()) & 0xFFFF;
    u2 = (U64)(generate_random_number()) & 0xFFFF;
    u3 = (U64)(generate_random_number()) & 0xFFFF;
    u4 = (U64)(generate_random_number()) & 0xFFFF;

    // shuffle bits and return
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

// get random few bits
U64 random_fewbits() {
    return random_U64() & random_U64() & random_U64();
}

// count bits (Brian Kernighan's way)
int count_bits(U64 bitboard) {
    // bit count
    int count = 0;

    // pop bits untill bitboard is empty
    while (bitboard)
    {
        // increment count
        count++;

        // consecutively reset least significant 1st bit
        bitboard &= bitboard - 1;
    }

    // return bit count
    return count;
}

// get index of LS1B in bitboard
int get_ls1b_index(U64 bitboard) {
    // make sure bitboard is not empty
    if (bitboard != 0)
        // convert trailing zeros before LS1B to ones and count them
        return count_bits((bitboard & -static_cast<long long>(bitboard)) - 1);

    // otherwise
    else
        // return illegal index
        return -1;
}

// set occupancies
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{
    // occupancy map
    U64 occupancy = 0ULL;

    // loop over the range of bits within attack mask
    for (int count = 0; count < bits_in_mask; count++)
    {
        // get LS1B index of attacks mask
        int square = get_ls1b_index(attack_mask);

        // pop LS1B in attack map
        pop_bit(attack_mask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }

    // return occupancy map
    return occupancy;
}

// mask bishop attacks
U64 mask_bishop_attacks(int square)
{
    // attack bitboard
    U64 attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int rk = square / 8;
    int fl = square % 8;

    // generate attacks
    for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (f + r * 8));
    for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (f + r * 8));
    for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (f + r * 8));
    for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (f + r * 8));
    // return attack map for bishop on a given square
    return attacks;
}

// mask rook attacks
U64 mask_rook_attacks(int square)
{
    // attacks bitboard
    U64 attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int rk = square / 8;
    int fl = square % 8;

    // generate attacks
    for (r = rk + 1; r <= 6; r++) attacks |= (1ULL << (fl + r * 8));
    for (r = rk - 1; r >= 1; r--) attacks |= (1ULL << (fl + r * 8));
    for (f = fl + 1; f <= 6; f++) attacks |= (1ULL << (f + rk * 8));
    for (f = fl - 1; f >= 1; f--) attacks |= (1ULL << (f + rk * 8));

    // return attack map for bishop on a given square
    return attacks;
}

// bishop attacks
U64 bishop_attacks_on_the_fly(int square, U64 block)
{
    // attack bitboard
    U64 result = 0ULL;

    int rk = square / 8, fl = square % 8, r, f;

    for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    return result;
}

// rook attacks
U64 rook_attacks_on_the_fly(int square, U64 block)
{
    U64 result = 0ULL;
    int rk = square / 8, fl = square % 8, r, f;
    for (r = rk + 1; r <= 7; r++) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    for (r = rk - 1; r >= 0; r--) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    for (f = fl + 1; f <= 7; f++) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    for (f = fl - 1; f >= 0; f--) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    return result;
}


/**************************************\

        Generating magic numbers

\**************************************/

// find magic number
U64 find_magic(int square, int relevant_bits, int bishop) {
    // define occupancies array
    U64 occupancies[4096];

    // define attacks array
    U64 attacks[4096];

    // define used indicies array
    U64 used_attacks[4096];

    // mask piece attack
    U64 mask_attack = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // occupancy variations
    int occupancy_variations = 1 << relevant_bits;

    // loop over the number of occupancy variations
    for (int count = 0; count < occupancy_variations; count++) {
        // init occupancies
        occupancies[count] = set_occupancy(count, relevant_bits, mask_attack);

        // init attacks
        attacks[count] = bishop ? bishop_attacks_on_the_fly(square, occupancies[count]) :
            rook_attacks_on_the_fly(square, occupancies[count]);
    }

    // test magic numbers
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // init magic number candidate
        U64 magic = random_fewbits();

        // skip testing magic number if inappropriate
        if (count_bits((mask_attack * magic) & 0xFF00000000000000ULL) < 6) continue;

        // reset used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init count & fail flag
        int count, fail;

        // test magic index
        for (count = 0, fail = 0; !fail && count < occupancy_variations; count++) {
            // generate magic index
            int magic_index = (int)((occupancies[count] * magic) >> (64 - relevant_bits));

            // if got free index
            if (used_attacks[magic_index] == 0ULL)
                // assign corresponding attack map
                used_attacks[magic_index] = attacks[count];

            // otherwise fail on  collision
            else if (used_attacks[magic_index] != attacks[count]) fail = 1;
        }

        // return magic if it works
        if (!fail) return magic;
    }

    // on fail
    printf("***Failed***\n");
    return 0ULL;
}

// init magics
//void init_magics()
//{
//    printf("const U64 rook_magics[64] = {\n");
//
//    // loop over 64 board squares
//    for (int square = 0; square < 64; square++)
//        printf("    0x%llxULL,\n", find_magic(square, rook_rellevant_bits[square], 0));
//
//    printf("};\n\nconst U64 bishop_magics[64] = {\n");
//
//    // loop over 64 board squares
//    for (int square = 0; square < 64; square++)
//        printf("    0x%llxULL,\n", find_magic(square, bishop_rellevant_bits[square], 1));
//
//    printf("};\n\n");
//}
//

/**************************************\

        Initializing attack table

\**************************************/

// rook magic numbers
const U64 rook_magics[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL,
};

const U64 bishop_magics[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL,
};

// init slider pieces attacks
void init_sliders_attacks(int is_bishop)
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        // init current mask
        U64 mask = is_bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

        // count attack mask bits
        int bit_count = count_bits(mask);

        // occupancy variations count
        int occupancy_variations = 1 << bit_count;

        // loop over occupancy variations
        for (int count = 0; count < occupancy_variations; count++)
        {
            // bishop
            if (is_bishop)
            {
                // init occupancies, magic index & attacks
                U64 occupancy = set_occupancy(count, bit_count, mask);
                U64 magic_index = occupancy * bishop_magics[square] >> 64 - bishop_rellevant_bits[square];
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }

            // rook
            else
            {
                // init occupancies, magic index & attacks
                U64 occupancy = set_occupancy(count, bit_count, mask);
                U64 magic_index = occupancy * rook_magics[square] >> 64 - rook_rellevant_bits[square];
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

// lookup bishop attacks 
U64 chess::get_bishop_attacks(int square, U64 occupancy) {

    // calculate magic index
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= 64 - bishop_rellevant_bits[square];

    // return rellevant attacks
    return bishop_attacks[square][occupancy];

}

// lookup rook attacks 
U64 chess::get_rook_attacks(int square, U64 occupancy) {

    // calculate magic index
    occupancy &= rook_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= 64 - rook_rellevant_bits[square];

    // return rellevant attacks
    return rook_attacks[square][occupancy];
}

