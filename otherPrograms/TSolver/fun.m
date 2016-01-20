function  y = fun(x) 
%#codegen
coder.inline('never');
y = (x-1.5).^3-1;

end