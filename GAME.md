# Snail Mail

Game Boy Color platformer. You are a snail. You deliver mail.

**Inspo:** Kirby’s Dream Land, Wario Land 2 (no death), Wario Land: Super Mario Land 3.

**Length:** ~45–60 min first clear. ~90 min with all stamps.

**Engine:** GB Studio 4.x platformer. Notes below are constraints, not a build plan.

---

## Premise

You are the valley’s only mail snail. Letters go missing — a Magpie keeps stealing the shiny ones. Find each letter and deliver it.

**Win:** Deliver all four letters and beat the Magpie at Parcel Peak.

**Win well:** Also collect the optional stamps. Stamp count changes the ending.

**Feel:** Soft and sturdy. Enemies are toys. Delivery is the point. Stamps are optional greed.

---

## Tone

- No death. Hits and pits only delay or divert you. Combat is optional when layout allows.

---

## Structure

| Chunk | Role | ~Time |
|-------|------|------:|
| Title / intro | Learn controls in play | 2 min |
| Level 1 — Daisy Downs | Letter 1 (meadows) | 8–10 min |
| Level 2 — Rose Ramble | Letter 2 (flower beds) | 8–10 min |
| Level 3 — Clothespin Cliffs | Letter 3 (backyard lines) | 8–10 min |
| Level 4 — Tick-Tock Terrace | Letter 4 (dusk towers) | 8–10 min |
| Parcel Peak | Magpie boss (only boss) | 5–8 min |
| Ending | By stamp count | — |

```mermaid
flowchart LR
  intro[Title / intro] --> L1[Daisy Downs]
  L1 --> L2[Rose Ramble]
  L2 --> L3[Clothespin Cliffs]
  L3 --> L4[Tick-Tock Terrace]
  L4 --> boss[Parcel Peak]
  boss --> ending[Ending]
```

**Optional stamps:** One rare stamp per level, off the main path. Not required. Available on every level (1–4). Stamp count changes the ending (shed → office → fancy sorting place).

---

## Core loop (per level)

1. **Find the letter** — mid-level, not at the start.
2. **Optional stamp** — one rare stamp off the main path. Skip freely.
3. **Deliver it** — reach the recipient. Short line or two + emote. Level clears. Next level unlocks.

**While carrying:** movement unchanged. Letter stays with you until delivery. Hits do not drop it. No drop button.

---

## Player verbs

### Move and jump
Walk, jump, coyote time.

### Shell Bump
Short forward bump on ground (brief air bump OK). Hits small enemies, soft blocks, secret bells.

---

## Movement kit

| Feature | Ship? | Notes |
|---------|-------|-------|
| Coyote time | Yes | |
| Double jump | No | |
| Drop-through platforms | Yes | |
| Moving / solid platforms | Sparingly | |
| Ladders | One level max | Clothespin Cliffs |
| Dash | No | |
| Wall jump | No | |
| Run | No | |

---

## Difficulty (no death)

Like Wario Land 2: you cannot die or game over from damage.

- **No HP. No hearts. No lives. No game over.**
- Enemy hit = knockback + short stun. Maybe bounce or brief flatten. Then continue.
- Pit / screen exit = soft recover to the nearest safe ledge (or bounce back). Not a restart.
- Challenge = delay, knockback into bad spots, boss patterns — not run failure.
- Most enemies patrol. Few chase.
- Combat is optional when a path exists around it.
- Short dialogue only.

---

## Enemies and bosses

Real garden threats. Four types max:

1. **Ant** — ground patrol.
2. **Beetle** — slow ground foe.
3. **Spider** — hangs or drops from above.
4. **Wasp** — slow flyer.

**One boss:** Magpie at Parcel Peak. Steals shiny mail.

- Peck / dive can knock you off the platform.
- No death. Soft recover to the arena start.
- Boss fight **restarts** (hit count resets).
- Win by landing enough hits in one clean attempt.

No midgame bosses.

---

## Presentation

- **Color:** Original Game Boy palette throughout.
- **Music:** One theme per level, delivery sting, boss track.
- **Title:** Logo + start. No lore scroll.
- **Ending:** Changes with stamp count. Same credits length.

---

## Non-goals

- Metroidvania backtracking.
- Hub / overworld revisit loop.
- Shops, tolls, coin gambling, score chase.
- Power-up sets, transformations, inventory screens.
- Slime trail physics.
- 15+ stages, secret worlds, New Game+.
- Hard mode as a day-one need.
- Midgame bosses.
- Death, HP, lives, game over, death checkpoints.

---

## GB Studio constraints

- Use platformer scenes. Enable only the moves you ship.
- Letters, recipients, stamps = actors, triggers, variables. No death/respawn state machine.
- Carry = a flag. Hits never drop the letter. No second movement mode.
- Hits = knockback / stun scripts. Pits = trigger warp or bounce to safe ground.
- Shell Bump = short scripted lunge or hit window.
- Keep actor count low. Split long levels across scenes.
- Save between levels is enough.

---

## Fun checklist

1. Finding the letter feels good.
2. Delivery is a clear beat.
3. Shell Bump is useful and a bit funny.
4. Stamps reward looking around.
5. Hits only slow you down.
6. The snail always feels like a snail.
