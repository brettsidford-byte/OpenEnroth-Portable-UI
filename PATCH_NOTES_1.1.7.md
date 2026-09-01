# OpenEnroth Portable UI 1.1.7

## Readable fonts

- Replace the previous subtle Readable Font effect with remastered bitmap fonts for the smallest main-interface text.
- Improve `lucida.fnt` status-bar messages, `smallnum.fnt` food and gold values, and `create.fnt` Rest-screen and shared interface text.
- Preserve every original glyph height, width, advance, offset and supported character while increasing stroke readability.
- Keep all replacement fonts as embedded Portable resources without modifying users' original `ICONS.LOD`.
- Add a clearer selected-state button for the Readable Font option.

## Interface improvements

- Remaster the Controls background while retaining the original control and overlay coordinates.
- Display option overlays only for the currently active turn-rate and toggle settings.
- Remaster the Rest-screen button surfaces while retaining localized runtime text.
- Draw the Portable Rest buttons above the stock interface artwork so the original controls cannot cover them.
- Remove obsolete Portable Colored Lights button assets after replacing that menu row with Readable Font.

## Android testing

- Give debug APKs a separate application ID and label so test and signed release builds can be installed side by side.
- Clean generated Android resources before debug builds to prevent stale resource collisions.

Portable replacements remain separate from users' original Might & Magic VII archives.
