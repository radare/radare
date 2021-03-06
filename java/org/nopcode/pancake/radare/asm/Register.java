/*
 * Copyright (C) 2006
 *       pancake <pancake@youterm.com>
 *
 * radare is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * radare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with radare; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

package org.nopcode.pancake.radare;

import org.nopcode.pancake.radare.RegisterType;

public class Register
{
  private int integer;
  RegisterType rt;

  public Register(RegisterType rt)
  {
	this.rt = rt;
  }

  public String toString()
  {
	switch(rt) {
	case INT32:
		System.out.println(integer);
	}
  }
}
