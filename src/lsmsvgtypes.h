/* Lasem - SVG and Mathml library
 *
 * Copyright © 2010 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_SVG_TYPES_H
#define LSM_SVG_TYPES_H

#include <lsmsvgenums.h>

G_BEGIN_DECLS

typedef struct _LsmSvgDocument LsmSvgDocument;
typedef struct _LsmSvgElement LsmSvgElement;
typedef struct _LsmSvgTransformable LsmSvgTransformable;
typedef struct _LsmSvgGraphic LsmSvgGraphic;
typedef struct _LsmSvgClipPathElement LsmSvgClipPathElement;
typedef struct _LsmSvgSvgElement LsmSvgSvgElement;
typedef struct _LsmSvgAElement LsmSvgAElement;
typedef struct _LsmSvgGElement LsmSvgGElement;
typedef struct _LsmSvgDefsElement LsmSvgDefsElement;
typedef struct _LsmSvgUseElement LsmSvgUseElement;
typedef struct _LsmSvgImageElement LsmSvgImageElement;
typedef struct _LsmSvgSymbolElement LsmSvgSymbolElement;
typedef struct _LsmSvgMarkerElement LsmSvgMarkerElement;
typedef struct _LsmSvgRectElement LsmSvgRectElement;
typedef struct _LsmSvgCircleElement LsmSvgCircleElement;
typedef struct _LsmSvgEllipseElement LsmSvgEllipseElement;
typedef struct _LsmSvgFilterElement LsmSvgFilterElement;
typedef struct _LsmSvgFilterPrimitive LsmSvgFilterPrimitive;
typedef struct _LsmSvgFilterBlend LsmSvgFilterBlend;
typedef struct _LsmSvgFilterColorMatrix LsmSvgFilterColorMatrix;
typedef struct _LsmSvgFilterComposite LsmSvgFilterComposite;
typedef struct _LsmSvgFilterConvolveMatrix LsmSvgFilterConvolveMatrix;
typedef struct _LsmSvgFilterDisplacementMap LsmSvgFilterDisplacementMap;
typedef struct _LsmSvgFilterFlood LsmSvgFilterFlood;
typedef struct _LsmSvgFilterGaussianBlur LsmSvgFilterGaussianBlur;
typedef struct _LsmSvgFilterImage LsmSvgFilterImage;
typedef struct _LsmSvgFilterMerge LsmSvgFilterMerge;
typedef struct _LsmSvgFilterMergeNode LsmSvgFilterMergeNode;
typedef struct _LsmSvgFilterMorphology LsmSvgFilterMorphology;
typedef struct _LsmSvgFilterOffset LsmSvgFilterOffset;
typedef struct _LsmSvgFilterSpecularLighting LsmSvgFilterSpecularLighting;
typedef struct _LsmSvgFilterTurbulence LsmSvgFilterTurbulence;
typedef struct _LsmSvgFilterTile LsmSvgFilterTile;
typedef struct _LsmSvgLineElement LsmSvgLineElement;
typedef struct _LsmSvgPolylineElement LsmSvgPolylineElement;
typedef struct _LsmSvgPolygonElement LsmSvgPolygonElement;
typedef struct _LsmSvgPathElement LsmSvgPathElement;
typedef struct _LsmSvgTextElement LsmSvgTextElement;
typedef struct _LsmSvgTspanElement LsmSvgTspanElement;
typedef struct _LsmSvgGradientElement LsmSvgGradientElement;
typedef struct _LsmSvgLinearGradientElement LsmSvgLinearGradientElement;
typedef struct _LsmSvgRadialGradientElement LsmSvgRadialGradientElement;
typedef struct _LsmSvgStopElement LsmSvgStopElement;
typedef struct _LsmSvgSwitchElement LsmSvgSwitchElement;
typedef struct _LsmSvgPatternElement LsmSvgPatternElement;
typedef struct _LsmSvgMaskElement LsmSvgMaskElement;

typedef struct _LsmSvgView LsmSvgView;
typedef struct _LsmSvgStyle LsmSvgStyle;

G_END_DECLS

#endif
