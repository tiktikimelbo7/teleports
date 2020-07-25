/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import QtMultimedia 5.0
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QtWebEngine 1.7

Item {
    id: viewer
    property bool pinchInProgress: zoomPinchArea.active
    property var mediaSource
    property real maxDimension
    property bool showThumbnail: true

    property bool isVideo: videoPreviewSource !== ""
    property bool isAudio: audioPreviewSource !== ""
    property bool isImage: photoPreviewSource !== ""
    property bool userInteracting: pinchInProgress || flickable.sizeScale != 1.0
    property bool fullyZoomed: flickable.sizeScale == zoomPinchArea.maximumZoom
    property bool fullyUnzoomed: flickable.sizeScale == zoomPinchArea.minimumZoom

    property alias paintedHeight: image.paintedHeight
    property alias paintedWidth: image.paintedWidth

    signal clicked()

    function zoomIn(centerX, centerY, factor) {
        flickable.scaleCenterX = centerX / (flickable.sizeScale * flickable.width);
        flickable.scaleCenterY = centerY / (flickable.sizeScale * flickable.height);
        flickable.sizeScale = factor;
    }

    function zoomOut() {
        if (flickable.sizeScale != 1.0) {
            flickable.scaleCenterX = flickable.contentX / flickable.width / (flickable.sizeScale - 1);
            flickable.scaleCenterY = flickable.contentY / flickable.height / (flickable.sizeScale - 1);
            flickable.sizeScale = 1.0;
        }
    }

    function reset() {
        if (viewer.isVideo) {
            // Intentionally pausing, not stopping, as resuming is faster.
            videoPreview.pause();
        } else if (viewer.isAudio) {
            audioOutput.pause();
        } else {
            zoomOut();
        }
    }

    UITK.ActivityIndicator {
        id: activity
        anchors.centerIn: parent
        visible: running && !viewer.isVideo && !viewer.isAudio
        running: image.status != Image.Ready
    }
    WebEngineProfile{
      id:webProfile
    }
    WebEngineView {
      id: webView
      profile:webProfile
      url: videoPreviewSource? videoPreviewSource : audioPreviewSource
      visible: viewer.isAudio || viewer.isVideo
      settings.showScrollBars: false
      anchors {
        left: parent.left
        top: parent.top
        right: parent.right
        bottom: parent.bottom
      }
    }
    PinchArea {
        id: zoomPinchArea
        anchors.fill: parent
        visible: viewer.isImage
        property real initialZoom
        property real maximumScale: 3.0
        property real minimumZoom: 1.0
        property real maximumZoom: 3.0
        property bool active: false
        property var center

        onPinchStarted: {
            if (viewer.isVideo || viewer.isAudio) return;
            active = true;
            initialZoom = flickable.sizeScale;
            center = zoomPinchArea.mapToItem(media, pinch.startCenter.x, pinch.startCenter.y);
            zoomIn(center.x, center.y, initialZoom);
        }
        function clamp(x, min, max){
          if (x < min)x = min;
          else if (x > max)x = max;
          return x;
        }

        onPinchUpdated: {
            if (viewer.isVideo || viewer.isAudio) return;
            var zoomFactor = clamp(initialZoom * pinch.scale, minimumZoom, maximumZoom);
            if(zoomFactor>flickable.sizeScale+0.2 ||zoomFactor<flickable.sizeScale-0.2){
              flickable.sizeScale = zoomFactor;
            }
        }
        onPinchFinished: {
            active = false;
        }

        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: media.width
            contentHeight: media.height
            contentX: (sizeScale - 1) * scaleCenterX * width
            contentY: (sizeScale - 1) * scaleCenterY * height
            interactive: !viewer.pinchInProgress

            property real sizeScale: 1.0
            property real scaleCenterX: 0.0
            property real scaleCenterY: 0.0
            Behavior on sizeScale {
                enabled: !viewer.pinchInProgress
                UITK.UbuntuNumberAnimation {duration: UITK.UbuntuAnimation.FastDuration}
            }
            Behavior on scaleCenterX {
                UITK.UbuntuNumberAnimation {duration: UITK.UbuntuAnimation.FastDuration}
            }
            Behavior on scaleCenterY {
                UITK.UbuntuNumberAnimation {duration: UITK.UbuntuAnimation.FastDuration}
            }

            Item {
                id: media

                width: flickable.width * flickable.sizeScale
                height: flickable.height * flickable.sizeScale

                Image {
                    id: image
                    anchors.fill: parent
                    asynchronous: true
                    cache: false
                    source: photoPreviewSource
                    sourceSize {
                        width: viewer.maxDimension
                        height: viewer.maxDimension
                    }
                    fillMode: Image.PreserveAspectFit
                    visible: viewer.showThumbnail
                    opacity: status == Image.Ready ? 1.0 : 0.0
                    Behavior on opacity { UITK.UbuntuNumberAnimation {duration: UITK.UbuntuAnimation.FastDuration} }

                }

                Image {
                    id: highResolutionImage
                    anchors.fill: parent
                    asynchronous: true
                    cache: false
                    // Load image using the GalleryStandardImageProvider to ensure EXIF orientation
                    source: flickable.sizeScale > 1.0 ? photoPreviewSource : ""
                    sourceSize {
                        width: width
                        height: height
                    }
                    fillMode: Image.PreserveAspectFit
                }
            }

            UITK.Icon {
                id: playIcon
                width: units.gu(5)
                height: units.gu(5)
                anchors.centerIn: parent
                name: "media-playback-start"
                color: Suru.foregroundColor
                opacity: 0.8
                visible: (viewer.isAudio || viewer.isVideo) && !activity.visible
            }

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    if (flickable.sizeScale < zoomPinchArea.maximumZoom) {
                        zoomIn(mouse.x, mouse.y, zoomPinchArea.maximumZoom);
                    } else {
                        zoomOut();
                    }
                }
            }
        }
    }
}
