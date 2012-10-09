Bioscope
========

Bioscope is a Qt wrapper for some ffmpeg functionality geared toward playback of framebased video file formats
(e.g. MJPEG, Dv compressions, and RAW video). The current goal is to replace Phonon based playback component in
Watch That Sound workshops tool (sound track composition and recording editor).

The main reason to get rid of Phonon is its dependency on DirectShow on Windows, which in our experience is often
misconfigured on user systems. We did our best to try to provide automagic DirectShow configuration fixer, but
eventually given up in favor of a custom solution.

WTS tool was already embedding ffmpeg for thumbnail generation and video export, so the switch only made the tool
lighter on dependencies.

(NB as of today (2012/10/09) the above statement is a projection, as the tool itself isn't ported to Bioscope yet)

