#include "precompiled.h"
#include "draw.h"
#include "data_cache.h"
#include "model.h"

Model::Model(const std::string &dir, const std::string &filename)
{
    tex_id = 0;
    tex_normal_id = 0;
    tex_relief_id = 0;
    load(dir, filename);

    if(tex_id != 0 && (tex_normal_id != 0 || tex_relief_id != 0))
        computeTBN();
}

void Model::computeTBN()
{
    tangents4.resize(verts.size());
    vertex_normals.resize(verts.size());
    std::vector<cml::vector3f> tangents(verts.size());
    std::vector<cml::vector3f> bitangents(verts.size());

    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        Vert &v0 = face.verts[0];
        Vert &v1 = face.verts[1];
        Vert &v2 = face.verts[2];

        cml::vector3f &p0 = verts[v0.vertex];
        cml::vector3f &p1 = verts[v1.vertex];
        cml::vector3f &p2 = verts[v2.vertex];

        cml::vector3f &t0 = tex_coords[v0.tex_coord];
        cml::vector3f &t1 = tex_coords[v1.tex_coord];
        cml::vector3f &t2 = tex_coords[v2.tex_coord];

        cml::vector3f q1 = p1 - p0;
        cml::vector3f q2 = p2 - p0;

        cml::vector3f st1 = t1 - t0;
        cml::vector3f st2 = t2 - t0;

        cml::vector3f n = cml::normalize(cml::cross(q1, q2));

        cml::matrix22f_c m_tex_coords;
        m_tex_coords(0,0) =  st2[1]; m_tex_coords(0,1) = -st1[1];
        m_tex_coords(1,0) = -st2[0]; m_tex_coords(1,1) =  st1[0];

        cml::matrix23f_c m_pts;
        m_pts(0,0) = q1[0]; m_pts(0,1) = q1[1]; m_pts(0,2) = q1[2]; 
        m_pts(1,0) = q2[0]; m_pts(1,1) = q2[1]; m_pts(1,2) = q2[2]; 

        float inv = 1.0f / (st1[0]*st2[1] - st2[0]*st1[1]);

        cml::matrix23f_c tb = inv * m_tex_coords * m_pts;
        cml::vector3f t(tb(0,0), tb(0,1), tb(0,2));
        cml::vector3f b(tb(1,0), tb(1,1), tb(1,2));

        vertex_normals[v0.vertex] += n;
        vertex_normals[v1.vertex] += n;
        vertex_normals[v2.vertex] += n;

        tangents[v0.vertex] += t;
        tangents[v1.vertex] += t;
        tangents[v2.vertex] += t;

        bitangents[v0.vertex] += b;
        bitangents[v1.vertex] += b;
        bitangents[v2.vertex] += b;
    }

    for(unsigned int i = 0; i < tangents.size(); i++)
    {
        cml::vector3f n = cml::normalize(vertex_normals[i].normalize());
        cml::vector3f t_total = tangents[i];
        cml::vector3f b_total = bitangents[i];

        cml::vector3f t = t_total - cml::dot(n, t_total) * n;
        t.normalize();
        cml::vector3f b = b_total - cml::dot(n, b_total) * n - cml::dot(t, b_total) * t;

        t.normalize();
        b.normalize();

        tangents[i]       = t;
        bitangents[i]     = b;
        vertex_normals[i] = n;

        cml::matrix33f_c tbn(
            t[0], t[1], t[2],
            b[0], b[1], b[2],
            n[0], n[1], n[2]
        );

        //print(format("det=%1%\n") % cml::determinant(tbn));
        tangents4[i] = cml::vector4f(t, cml::determinant(tbn));
    }
}

void Model::draw()
{
    if(tex_id)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_id);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    //glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        for(unsigned int vert_i = 0; vert_i < 3; vert_i++)
        {
            Vert &vert = face.verts[vert_i];

            cml::vector3f &pos = verts[vert.vertex];
            cml::vector3f &norm = norms[vert.normal];

            if(vert.tex_coord != -1 && tex_id)
            {
                cml::vector3f &tex_coord = tex_coords[vert.tex_coord];
                glTexCoord2fv(tex_coord.data());
            }

            glNormal3fv(norm.data());
            glVertex3fv(pos.data());
        }
    }
    glEnd();
}

void Model::drawNormals(float scale)
{
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        for(unsigned int vert_i = 0; vert_i < 3; vert_i++)
        {
            Vert &vert = face.verts[vert_i];

            cml::vector3f &p0 = verts[vert.vertex];
            cml::vector3f p1 = p0 + norms[vert.normal]*scale;


            glVertex3fv(p0.data());
            glVertex3fv(p1.data());
        }
    }
    glEnd();

    glPopAttrib();
}

void Model::drawTBNs(float scale)
{
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        for(unsigned int vert_i = 0; vert_i < 3; vert_i++)
        {
            Vert &vert = face.verts[vert_i];

            cml::vector3f &p = verts[vert.vertex];
            cml::vector3f  t = cml::vector3f(tangents4[vert.vertex].data()) * scale;
            cml::vector3f  n = vertex_normals[vert.vertex] * scale;
            cml::vector3f  b = cml::normalize(cml::cross(n, t)) * tangents4[vert.vertex][3] * scale;

            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3fv(p.data()); glVertex3f(p[0]+t[0], p[1]+t[1], p[2]+t[2]);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3fv(p.data()); glVertex3f(p[0]+b[0], p[1]+b[1], p[2]+b[2]);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3fv(p.data()); glVertex3f(p[0]+n[0], p[1]+n[1], p[2]+n[2]);
        }
    }
    glEnd();

    glPopAttrib();
}

void Model::load(const std::string &dir, const std::string &filename)
{
    std::ifstream in((dir + filename).c_str());

    if(!in.is_open())
    {
        warning(format("failed to load model \"%1%\"\n") % filename);
        return;
    }

    while(in.good())
    {
        std::string line;
        std::getline(in, line);
        //std::cout << line << "\n";

        if(boost::starts_with(line, "v "))
        {
            boost::replace_first(line, "v ", "");
            std::istringstream iss(line);

            cml::vector3f v;
            iss >> v[0];
            iss >> v[1];
            iss >> v[2];
            verts.push_back(v);
        }
        else if(boost::starts_with(line, "vt "))
        {
            boost::replace_first(line, "vt ", "");
            std::istringstream iss(line);

            cml::vector3f v;
            iss >> v[0];
            iss >> v[1];
            tex_coords.push_back(v);
        }
        else if(boost::starts_with(line, "vn "))
        {
            boost::replace_first(line, "vn ", "");
            std::istringstream iss(line);

            cml::vector3f v;
            iss >> v[0];
            iss >> v[1];
            iss >> v[2];
            norms.push_back(v);
        }
        else if(boost::starts_with(line, "f ") && boost::contains(line, "//"))
        {
            boost::replace_first(line, "f ", "");
            boost::replace_all(line, "/", " ");
            std::istringstream iss(line);

            Face face;
            for(int i = 0; i < 3; i++)
            {
                iss >> face.verts[i].vertex;
                iss >> face.verts[i].normal;

                face.verts[i].vertex--;
                face.verts[i].normal--;

                face.verts[i].tex_coord = -1;
            }
            faces.push_back(face);
        }
        else if(boost::starts_with(line, "f "))
        {
            boost::replace_first(line, "f ", "");
            boost::replace_all(line, "/", " ");
            std::istringstream iss(line);

            Face face;
            for(int i = 0; i < 3; i++)
            {
                iss >> face.verts[i].vertex;
                iss >> face.verts[i].tex_coord;
                iss >> face.verts[i].normal;

                face.verts[i].vertex--;
                face.verts[i].tex_coord--;
                face.verts[i].normal--;

            }
            faces.push_back(face);
        }
        else if(boost::starts_with(line, "mtllib "))
        {
            boost::replace_first(line, "mtllib ", "");
            std::istringstream iss(line);
            std::string mtl_filename;
            iss >> mtl_filename;
            loadMaterial(dir, mtl_filename);
        }

    }
    in.close();
}

void Model::loadMaterial(const std::string &dir, const std::string &filename)
{
    std::ifstream in((dir + filename).c_str());

    if(!in.is_open())
        return;

    while(in.good())
    {
        std::string line;
        std::getline(in, line);
        //std::cout << line << "\n";

        if(boost::starts_with(line, "map_Kd "))
        {
            boost::replace_first(line, "map_Kd ", "");
            std::istringstream iss(line);
            std::string tex_filename;
            iss >> tex_filename;
            tex_id = DataCache::getTexture(dir + tex_filename);
        }
        else if(boost::starts_with(line, "map_normal "))
        {
            boost::replace_first(line, "map_normal ", "");
            std::istringstream iss(line);
            std::string tex_filename;
            iss >> tex_filename;
            tex_normal_id = DataCache::getTexture(dir + tex_filename);
        }
        else if(boost::starts_with(line, "map_relief "))
        {
            boost::replace_first(line, "map_relief ", "");
            std::istringstream iss(line);
            std::string tex_filename;
            iss >> tex_filename;
            tex_relief_id = DataCache::getTexture(dir + tex_filename);
        }
    }
    in.close();
}

