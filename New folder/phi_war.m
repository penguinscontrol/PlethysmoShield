function [ out_phi ] = phi_war( d,pw )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
if (ismember(d,[1,2,3,4,5]))
    
if pw == 10 && d == 1
    out_phi = 0.248766;
end
if pw == 10 && d == 2
    out_phi = 0.049559;
end
if pw == 10 && d == 3
    out_phi = 0.008374;
end
if pw == 10 && d == 4
    out_phi = 0.001259;
end
if pw == 10 && d == 5
    out_phi = 0.000174;
end

if pw == 100 && d == 1
    out_phi = 0.442054;
end
if pw == 100 && d == 2
    out_phi = 0.195067;
end
if pw == 100 && d == 3
    out_phi = 0.085843;
end
if pw == 100 && d == 4
    out_phi = 0.037606;
end
if pw == 100 && d == 5
    out_phi = 0.016353;
end

else
    out_phi = 0;
end
end

