﻿using BindingsKernel;
using CelesteEditor.Assets;
using CelesteEditor.Core;
using CelesteEditor.Extensibility;
using System;
using System.IO;
using System.Xml;
using System.Xml.Serialization;
using static CelesteEditor.Assets.ScriptableObjectAsset;

namespace CelesteEditor.AssetLoaders
{
    public class PrefabAssetLoader : AssetLoader
    {
        #region Properties and Fields

        /// <summary>
        /// The file extension which this loader is responsible for loading into a project asset.
        /// </summary>
        public const string FileExtension = ".prefab";

        /// <summary>
        /// The file extension which this loader is responsible for loading into a project asset.
        /// </summary>
        public override string AssetFileExtension { get { return FileExtension; } }

        #endregion

        #region Load Functions

        public override IAsset Load(FileInfo file)
        {
            Prefab prefab = new Prefab();
            prefab.Path = file.FullName;

            return new PrefabAsset(file, prefab);
        }

        #endregion
    }
}