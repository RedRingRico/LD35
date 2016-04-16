bl_info = {
    "name":         "LD Model",
    "author":       "Rico Tyrell",
    "blender":      ( 2, 6, 2 ),
    "version":      ( 0, 0, 1 ),
    "location":     "File > Import-Export",
    "description":  "Export as a LD model",
    "category":     "Import-Export"
}

import bpy;
import struct;
import io;
import os;
from bpy_extras.io_utils import ExportHelper

def menu_func( self, context ):
    self.layout.operator( Exporter.bl_idname, text="LD Model format (.ld)" );

def register( ):
    bpy.utils.register_module( __name__ );
    bpy.types.INFO_MT_file_export.append( menu_func );

def unregister( ):
    bpy.utils.unregister_module( __name__ );
    bpy.types.INFO_MT_file_export.remove( menu_func );

if __name__ == "__main__":
    register( );

class FileHeader:
    MeshCount       = 0;
    JointCount      = 0;

    def __init__( self, MeshCount, JointCount ):
        self.MeshCount = MeshCount;
        self.JointCount = JointCount;

    def write( self, file ):
        file.write( struct.pack( '<cccc', b'F', b'P', b'S', b'M' ) );
        file.write( struct.pack( "<I", self.MeshCount ) );
        file.write( struct.pack( "<I", self.JointCount ) );

class Exporter( bpy.types.Operator, ExportHelper ):
    bl_idname       = "untitled.ld";
    bl_label        = "Export LD";
    bl_options      = { 'PRESET' };
    mesh_list       = [ ];
    filename_ext    = ".ld";
    
    def execute( self, context ):
        del self.mesh_list[ 0:len( self.mesh_list ) ];
        bpy.ops.object.mode_set( mode='OBJECT' );
        result = { 'FINISHED' };
        for object in bpy.data.objects:
            if object.type == 'MESH':
                self.mesh_list.append( object );
        file = open( self.filepath, 'wb' );
        fileHeader = FileHeader( len( self.mesh_list ), 0 );
        fileHeader.write( file );
        for mesh in self.mesh_list:
            WriteMeshChunk( file, mesh );
        file.close( );
        return result;

def WriteEndChunk( File ):
    File.write( struct.pack( "<I", 0xFFFFFFFF ) );
    File.write( struct.pack( "<I", 0 ) );

def WriteMeshChunk( File, Mesh ):
    File.write( struct.pack( "<I", 0x00000001 ) );
    # This will be replaced later in the funciton with the file size
    File.write( struct.pack( "<I", 0 ) );
    FileSize = 0;
    for x in range( 0, 64 ):
        File.write( struct.pack( "<c", b'?' ) );
    FileSize += 64;
    File.write( struct.pack( "<I", len( Mesh.data.vertices ) ) );
    FileSize += 4;
    for x in range( 0, 16 ):
        File.write( struct.pack( "<c", b'Z' ) );
    FileSize += 16;
    for vert in Mesh.data.vertices:
        Co = Mesh.matrix_world * vert.co;
        File.write( struct.pack( "<fff", Co[ 0 ], Co[ 2 ], -Co[ 1 ] ) );
        FileSize += 12;
        File.write( struct.pack( "<fff", vert.normal[ 0 ], vert.normal[ 2 ], -vert.normal[ 1 ] ) );
        FileSize += 12;
        File.write( struct.pack( "<ff", 0.0, 0.0 ) );
        FileSize += 8;
        File.write( struct.pack( "<iiii", 0, 0, 0, 0 ) );
        FileSize += 16;
        File.write( struct.pack( "<ffff", 0.0, 0.0, 0.0, 0.0 ) );
        FileSize += 16;
    IndexOffset = 0;
    IndexCount = 0;
    for face in Mesh.data.polygons:
        face_verts = face.vertices;
        if( len( face.vertices ) == 3 ):
            File.write( struct.pack( "<HHH", face_verts[ 2 ], face_verts[ 1 ], face_verts[ 0 ] ) );
            FileSize += 6;
            IndexOffset += 6;
            IndexCount += 3;
        else:
            File.write( struct.pack( "<HHH", face_verts[ 2 ], face_verts[ 1 ], face_verts[ 0 ] ) );
            File.write( struct.pack( "<HHH", face_verts[ 3 ], face_verts[ 2 ], face_verts[ 0 ] ) );
            FileSize += 12;
            IndexOffset += 12;
            IndexCount += 6;
    File.seek( -( IndexOffset + 4 ), 1 );
    File.write( struct.pack( "<I", IndexCount ) );
    File.seek( IndexOffset, 1 );
    File.seek( -( FileSize  + 4 ), 1 );
    File.write( struct.pack( "<I", FileSize ) );
    File.seek( FileSize, 1 );
    WriteEndChunk( File );
