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
%% File: network_def.m: Definition of the Network Module
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [PN_name, set_of_places, set_of_trans, set_of_arcs] ...
    = network_def(global_info)

PN_name = 'Network Module';
set_of_places = {'pTX', 'pRX'};
set_of_trans = {'tTX', 'tRX'};
set_of_arcs = { ...
    'tTX','pTX',1, ...
    'pRX','tRX',1 ...
    };
