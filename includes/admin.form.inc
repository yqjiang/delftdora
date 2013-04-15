<?php

/**
 * @file
 *
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function delft_admin($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $mods_xslt = variable_get('delft_mods_transform_xslt', FALSE);
  $mods_message = $mods_xslt ? 'Change MODS XSLT' : 'Add MODS XSLT';
  if ($mods_xslt) {
    $form['mods_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current MODS XSLT'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['mods_wrapper']['current'] = array(
      '#type' => 'item',
      '#markup' => "<pre>" . htmlentities("$mods_xslt") . "</pre>",
    );
  }

  $form['mods_xslt'] = array(
    '#type' => 'file',
    '#title' => t($mods_message),
    '#description' => t('Change xslt used for MODS transformations.')
  );
  $rels_xslt = variable_get('delft_rels_transform_xslt', FALSE);
  $rels_message = $rels_xslt ? 'Change RELS-EXT XSLT' : 'Add RELS-EXT XSLT';
  if ($rels_xslt) {
    $form['rels_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current RELS-EXT XSLT'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['rels_wrapper']['current'] = array(
      '#type' => 'item',
      '#markup' => "<pre>" . htmlentities("$rels_xslt") . "</pre>",
    );
  }

  $form['rels_xslt'] = array(
    '#type' => 'file',
    '#title' => t($rels_message),
    '#description' => t('Change xslt used for rels transformations.')
  );

  $sfoaf_xslt = variable_get('delft_sfoaf_transform_xslt', FALSE);
  $sfoaf_message = $sfoaf_xslt ? 'Change SFOAF XSLT' : 'Add SFOAF XSLT';
  if ($sfoaf_xslt) {
    $form['sfoaf_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current sfoaf XSLT'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['sfoaf_wrapper']['current'] = array(
      '#type' => 'item',
      '#markup' => "<pre>" . htmlentities("$sfoaf_xslt") . "</pre>",
    );
  }

  $form['sfoaf_xslt'] = array(
    '#type' => 'file',
    '#title' => t($sfoaf_message),
    '#description' => t('Change xslt used for sfoaf transformations.')
  );

  $form['table_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Content Models'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['table_wrapper']['the_table'] = islandora_content_model_select_table_form_element('delft_content_models');
  $form['islandora_transform_submit'] = array(
    '#value' => t('Save Configuration'),
    '#type' => 'submit',
  );
  return $form;
}

/**
 * admin fomr validator
 *
 * @param <type> $form
 * @param <type> $form_state
 */

function delft_admin_validate($form, &$form_state) {
  if (isset($form_state['values'])) {
    $validators = array('file_validate_extensions' => array('xsl', 'xslt'));
    $mods_file = file_save_upload('mods_xslt', $validators);
    if ($mods_file) {
      if ($mods_file = file_move($mods_file, 'public://')) {
        $form_state['storage']['mods_file'] = $mods_file;
      }
      else {
        form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
      }
    }
    $rels_file = file_save_upload('rels_xslt', $validators);
    if ($rels_file) {
      if ($rels_file = file_move($rels_file, 'public://')) {
        $form_state['storage']['rels_file'] = $rels_file;
      }
      else {
        form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
      }
    }
    $sfoaf_file = file_save_upload('sfoaf_xslt', $validators);
    if ($sfoaf_file) {
      if ($sfoaf_file = file_move($sfoaf_file, 'public://')) {
        $form_state['storage']['sfoaf_file'] = $sfoaf_file;
      }
      else {
        form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
      }
    }
  }
}

/**
 * admin form submit
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function delft_admin_submit($form, &$form_state) {
  if (isset($form_state['storage']['mods_file'])) {
    $file = $form_state['storage']['mods_file'];
    unset($form_state['storage']['mods_file']);
    variable_set("delft_mods_transform_xslt", file_get_contents($file->uri));
  }
  if (isset($form_state['storage']['rels_file'])) {
    $file = $form_state['storage']['rels_file'];
    unset($form_state['storage']['rels_file']);
    variable_set("delft_rels_transform_xslt", file_get_contents($file->uri));
  }
  if (isset($form_state['storage']['sfoaf_file'])) {
    $file = $form_state['storage']['sfoaf_file'];
    unset($form_state['storage']['sfoaf_file']);
    variable_set("delft_sfoaf_transform_xslt", file_get_contents($file->uri));
  }
  $enabled = array_filter($form_state['values']['the_table']);
  variable_set('delft_content_models', $enabled);
}

