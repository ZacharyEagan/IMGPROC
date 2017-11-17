import cv2

#image = cv2.imread("path", flag) #flag opt: cv2.IMREAD_COLOR | GRAYSCALE | UNCHANGED

camera = cv2.VideoCapture(1) #same camera numbering as c++ of course this is external webcam or camera 2 vs the nonfunctional built in



cont = True
while (cont):
	ret, img = camera.read()
	if (ret):
		cv2.imwrite("image.png", img)
		cv2.imshow('image', img)
	cont = (chr(27) != chr(cv2.waitKey(30) & 255))
		
	
cv2.destroyAllWindows()

