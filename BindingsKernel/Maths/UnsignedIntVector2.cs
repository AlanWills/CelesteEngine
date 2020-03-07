﻿using BindingsKernel.Serialization;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace BindingsKernel
{
    [Serializable]
    public class UnsignedIntVector2 : ICustomSerialization, ICustomDeserialization
    {
        #region Properties and Fields

        /// <summary>
        /// The X component of this vector.
        /// </summary>
        public uint X { get; set; }

        /// <summary>
        /// The Y component of this vector.
        /// </summary>
        public uint Y { get; set; }

        #endregion

        #region Constructors

        public UnsignedIntVector2() : this(0, 0)
        {
            // Needed for deserialization so don't delete
        }

        public UnsignedIntVector2(uint x, uint y)
        {
            X = x;
            Y = y;
        }

        #endregion

        #region ICustomSerialization Implementation

        public void Deserialize(string name, XmlReader reader)
        {
            string text = reader.GetAttribute(name);
            if (!string.IsNullOrEmpty(text))
            {
                int indexOfComma = text.IndexOf(',');

                uint x = 0, y = 0;
                uint.TryParse(text.Substring(0, indexOfComma), out x);
                uint.TryParse(text.Substring(indexOfComma + 1), out y);

                X = x;
                Y = y;
            }
        }

        public void Serialize(string name, XmlWriter writer)
        {
            StringBuilder builder = new StringBuilder(16);
            builder.Append(X);
            builder.Append(',');
            builder.Append(Y);

            writer.WriteAttributeString(name, builder.ToString());
        }

        #endregion
    }
}