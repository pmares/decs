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
%% File: tTX_def.m: Definition of the Network Transmit Transition
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [fire, global_info] = tTX_impl (PN, global_info)

% Ensures that pTX does not go above it's maximum limit.
p = get_place(PN, 'pTX');
fire = (p.tokens < global_info.ptx_max);
