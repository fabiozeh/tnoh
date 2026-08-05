W, H = 100, 31
canvas = [[' '] * W for _ in range(H)]

def place(sprite, row, col, opaque=False):
    lines = sprite.split('\n')
    if opaque:
        lo = min(len(l) - len(l.lstrip()) for l in lines if l.strip())
        hi = max(len(l.rstrip()) for l in lines)
        for r in range(len(lines)):
            for c in range(lo, hi):
                rr, cc = row + r, col + c
                if 0 <= rr < H and 0 <= cc < W:
                    canvas[rr][cc] = ' '
    for r, line in enumerate(lines):
        for c, ch in enumerate(line):
            if ch == ' ':
                continue
            rr, cc = row + r, col + c
            if 0 <= rr < H and 0 <= cc < W:
                canvas[rr][cc] = ch

def S(s):
    return s[1:].rstrip('\n')

GROUND = 30

# -- 1. the surgeon: funnel tipped right, straight spout at its end
surgeon = S(r"""
              ___
             /  /
            /  /
           /__/
        //     \
       /       |
      /        \
     /__________\
        ,-----.
       / -  - |
       |  ^   |
       \ ---  /
        '----'
    ____|_|____
   /           \
  /   |     |   \
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |    |     |    |
 |____|_____|____|
""")

arm = S(r"""
\_______>
""")

# the friar's right hand, raised over the patient toward the surgeon
gesture = S(r"""
 ,,,,
(    \
 \__/
   \\
     \\
       \\
         \
""")

# -- 2. the patient: slumped low in his chair, head lolling left
chair = S(r"""
.-----------------------.
|                       |
|                       |
|                       |
""")

patient = S(r"""
  \|/
   *
.--|-----.
 | x    x |
  |   v    |
   |  (_)  |
    \       /
     '-----'
  ____|_|_____
 /             \
/   |       |   \
|   |       |   |
|   |       |   |
|   |       |   |
|___|_______|___|
""")

# -- 3. the friar, pewter in his left hand
friar = S(r"""
     .-----.
    /  ___  \
    | o   o |
    |   -   |
    \  ~~~  /
     '-----'
   ____|_|____
  /           \
 /   |     |   \
|    |     |    |
|    |     |    |
|    |     |    |
|    |     |    |
|    |     |    |
|    |     |    |
|    |     |    |
|    |     |    |
|____|_____|____|
""")

jug = S(r"""
 ____
,|    |
'|    |
 |____|
""")

# -- 4. the nun, head propped on her right hand, book closed on her head
nun = S(r"""
     ________
    /_______/|
    |_______|'
     .-----.\
    |o    o| \
   (|  <   |  \
  / |\ --- /   \
 /   | '---'    |
 |  __|_|____   |
 | /          \ |
 |/   |    |   \|
 |    |    |   ||
 |    |    |   ||
""")

table = S(r"""
.--------------------------.
|__________________________|
 ||                      ||
 ||                      ||
 ||                      ||
 ||                      ||
 ||                      ||
 ||                      ||
""")

tulip = S(r"""
(@)
 |
""")

place(surgeon,  4,  0, opaque=True)
place(chair,   26, 20)
place(patient, 15, 24, opaque=True)
place(arm,     17, 15)
place(friar,   12, 46, opaque=True)
place(gesture, 15, 36)
place(jug,     21, 63, opaque=True)
place(nun,      9, 74, opaque=True)
place(tulip,   20, 95)
place(table,   22, 72, opaque=True)

for c in range(0, W):
    canvas[GROUND][c] = '_'

rows = [''.join(r).rstrip() for r in canvas]
while rows and not rows[0]:
    rows.pop(0)
out = '\n'.join(rows)
print(out)
open('/tmp/claude-1000/-home-fabio-projects-fun-blog/f5ab247a-e898-47e6-9153-d34cde571342/scratchpad/bosch2.txt', 'w').write(out + '\n')
