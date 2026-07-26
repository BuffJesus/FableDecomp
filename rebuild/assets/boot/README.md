# Boot artwork

`fabledecomp_boot_concept.png` is the project-owner-provided concept for the
first visual reconstruction screen.

- Canvas: 1672 x 941 pixels
- Format: PNG
- SHA-256: `E7F47344FE5FED2FE2B73779F068A2D408792A38CE863A2FBFC9F89BED47BCFF`
- Status: wired into the authored visual boot checkpoint

Keep the original PNG unchanged. Runtime-sized, compressed, or platform-native
derivatives are generated into the ignored build tree. The current VC7.1
bootstrap converts it to BMP and embeds it in the visual-checkpoint executable;
the recovered renderer may choose a different texture format later.
