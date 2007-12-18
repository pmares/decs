%%% DECS GPenSIM - Generic Distributed Exact Cover Solver (DECS) Petri net model
%%% Copyright (C) 2007  Jan Magne Tjensvold
%%% 
%%% This program is free software; you can redistribute it and/or modify it
%%% under the terms of the GNU General Public License version 2 as published
%%% by the Free Software Foundation.
%%% 
%%% This program is distributed in the hope that it will be useful, but
%%% WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
%%% General Public License for more details.
%%% 
%%% You should have received a copy of the GNU General Public License along
%%% with this program; if not, write to the Free Software Foundation, Inc.,
%%% 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
%%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% File: client_def.m: Definition of the Client Module
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [PN_name, set_of_places, set_of_trans, set_of_arcs] ...
    = client_def(global_info)

PN_name = 'Client Module';
set_of_places = {};
set_of_trans = {};
set_of_arcs = {};

for i=1:global_info.clients,
    set_of_places = [set_of_places, ['pJOBS', num2str(i)], ['pFREE', num2str(i)], ['pSOL', num2str(i)]];
    set_of_trans = [set_of_trans, ['tGET', num2str(i)], ['tCOMP', num2str(i)], ['tPUT', num2str(i)]];
    set_of_arcs = [ ...
        set_of_arcs, ...
        ['tGET', num2str(i)], ['pJOBS', num2str(i)],1, ...
        ['pJOBS', num2str(i)], ['tCOMP', num2str(i)],1, ...
        ['tCOMP', num2str(i)], ['pFREE', num2str(i)],1, ...
        ['pFREE', num2str(i)], ['tGET', num2str(i)],1, ...
        ['tCOMP', num2str(i)], ['pSOL', num2str(i)],1, ...
        ['pSOL', num2str(i)], ['tPUT', num2str(i)],1, ...
        ['tPUT', num2str(i)], ['pRX'],1, ...
        ['pTX'], ['tGET', num2str(i)],1, ...
        ];
end;
