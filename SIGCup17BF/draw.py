import cv2
import numpy as np
f = open('drawdata.txt', 'r')
[n, m] = [int(x) for x in f.readline().strip().split(' ')]
img = np.zeros((n, m, 3), np.uint8)
img[:][:][:] = 255

while True:
	point = [int(x) for x in f.readline().strip().split(' ') if len(x) > 0]
	if len(point) < 8:
		break
	if point[0] == point[2] and point[0] == point[4] and point[0] == point[6]:
		if point[1] == point[3] and point[1] == point[5] and point[1] == point[7]:
			continue
	point = [[point[1], point[0]], [point[3], point[2]], [point[5], point[4]], [point[7], point[6]]]
	#print 'draw', point
	cv2.fillConvexPoly(img, np.array(point, 'int32'), (0, 0, 255))
cv2.fillConvexPoly(img, np.array([[0, 0], [0, 29], [m, 29], [m, 0]], 'int32'), (255, 255, 255))
cv2.fillConvexPoly(img, np.array([[0, 0], [29, 0], [29, n], [0, n]], 'int32'), (255, 255, 255))
for i in xrange(1, n / 30):
	for j in xrange(1, m / 30):
		FF = [[0, 0], [0, 1], [1, 0], [0, - 1], [- 1, 0]]
		for x, y in FF:
			x += i * 30
			y += j * 30
			if x < 0 or x >= n or y < 0 or y >= m:
				continue
			img[x][y] = [0, 0, 0]

#print img.shape
#cv2.fillConvexPoly(img, np.array([[9930, 6270], [9960, 6240], [9930, 6240], [9930, 6240]], 'int32'), (0, 0, 255))
#print len(img[img != (255, 255, 255)])

cv2.imwrite('res.png', img)