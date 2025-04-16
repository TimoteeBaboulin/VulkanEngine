# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# TODO: Ideally this should look for each Qt module separately, with each module's specific version,
# bypassing the Qt6 Config file, aka find_package(Qt6SpecificFoo) repated x times. But it's not
# critical.
find_package(Qt6 6.9.0
             COMPONENTS 3DCore 3DLogic 3DInput 3DRender 3DExtras 3DAnimation 3DQuick 3DQuickRender 3DQuickScene2D 3DQuickScene3D 3DQuickExtras 3DQuickInput 3DQuickAnimation)
