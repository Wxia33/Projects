function nn = nnImResize(image)
im = imread(image);
%im = rgb2gray(im);
im = imresize(im,[20,20]);
im = (double(im));
im = double(im);
%im(im==0) = 255;
%im = (im-127)/128;
imshow(im)
nn = im(:)';