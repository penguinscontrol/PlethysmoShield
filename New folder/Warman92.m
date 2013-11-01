clear;clc;close all;
PW1 = 10;
PW2 = 100;
Phi_1 = 5.63e-6;
Phi_2 = 8.87e-6;
N_NODES = 23;
D = 12e-6;
L = 100*D;
E2F_DIST = 1e-3;
iamp = 1;
rho_e = 3;
Ga = (54.7e-2).^(-1);
d = 0.6*D;
l = 1.5e-6;

x_axon = [0:1:(N_NODES-1)].*L-ones(1,N_NODES).*0.5.*L.*(N_NODES-1);
rad = sqrt(x_axon.^2+(ones(1,N_NODES).*E2F_DIST).^2);
v_ext = -iamp.*(rad.^(-1))./(4*pi*rho_e);
act_fun = (diff(v_ext,2)).*(Ga./(pi.*d.*l));

T10 = act_fun;
this_sum = 0;
for a = 1:(N_NODES-2)
    for b = 1:(N_NODES-2)
        if (b ~= a)
        this_sum = this_sum+phi_war(abs(a-b),PW1).*act_fun(b);
        end
    end
    T10(a) = T10(a)+this_sum;
    this_sum = 0;
end

T100 = act_fun;
this_sum = 0;
for a = 1:(N_NODES-2)
    for b = 1:(N_NODES-2)
        if (b ~= a)
        this_sum = this_sum+phi_war(abs(a-b),PW2).*act_fun(b);
        end
    end
    T100(a) = T100(a)+this_sum;
    this_sum = 0;
end

figure(1);
plot(x_axon(2:(end-1)),act_fun,'ko-');
hold on;
plot(x_axon(2:(end-1)),T10,'rd-');
plot(x_axon(2:(end-1)),T100,'bs-');
legend('f','T(10)','T(100)');
%line(1.1.*[x_axon(2),x_axon(end-1)],[0,0]);
grid on;

v_m10 = T10.*Phi_1.*pi.*d.*l;
v_m100 = T100.*Phi_2.*pi.*d.*l;
figure(2);
hold on;
plot(x_axon(2:(end-1)),v_m10,'rd-');
plot(x_axon(2:(end-1)),v_m100,'bs-');
legend('V_m(100)','v_m(10)')
grid on