/*
 * Copyright (c) 2007, Laminar Research.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef WED_OverlayImage_H
#define WED_OverlayImage_H

#include "WED_GISPolygon.h"
#include "IGIS.h"

class WED_OverlayImage : public WED_GISPolygon, public virtual IGISQuad {

DECLARE_PERSISTENT(WED_OverlayImage)

public:

	void		GetImage(string& image_file) const;
	void		SetImage(const string& image_file);

	virtual	void	GetCorners(Point2 corners[4]) const;

	virtual	void	MoveCorner(int corner, const Vector2& delta);
	virtual	void	MoveSide(int side, const Vector2& delta);

	virtual	void	ResizeSide(int side, const Vector2& delta, bool symetric);
	virtual	void	ResizeCorner(int side, const Vector2& delta, bool symetric);

private:

	WED_PropFileText			mImageFile;

};

#endif /* WED_OverlayImage_H */