#!/bin/sh
cat <<EOF
[Desktop Entry]
Encoding=UTF-8
Version=$2
Type=Application
Terminal=False
Name=MARMEAS
Comment=Image Centric Object Marking and Measurement application
Exec=marmeas &
Categories=Application;Graphics;
X-AppInstall-Package=MARMEAS
Icon=marmeas.png
EOF

