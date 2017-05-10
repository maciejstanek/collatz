#!/bin/bash

points="100,100 200,200 300,200 400,300 400,400 500,500"
cb=#45961b
cf=#67bf39
# http://www.imagemagick.org/discourse-server/viewtopic.php?t=26666

convert -size 600x600 \
	xc:white \
	-fill none -stroke $cb -strokewidth 20 -draw "stroke-linecap round bezier $points" \
	-fill $cb -stroke none -draw "circle 500,500 510,500" \
	-fill none -stroke $cf -strokewidth 16 -draw "stroke-linecap round bezier $points" \
	-fill $cf -stroke none -draw "circle 500,500 508,500" \
	out.png

eog out.png
